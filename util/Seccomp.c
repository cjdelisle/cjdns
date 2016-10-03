/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// sigaction() siginfo_t SIG_UNBLOCK
#define _POSIX_C_SOURCE 199309L

#include "util/Seccomp.h"
#include "util/Bits.h"
#include "util/ArchInfo.h"
#include "util/Defined.h"

// getpriority()
#include <sys/resource.h>
#include <signal.h>
#include <sys/prctl.h>
#include <errno.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/audit.h>
#include <linux/netlink.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/**
 * A unique number which is returned as errno by getpriority(), a syscall we never use
 * this will be used by Seccomp_isWorking() to detect that the filter has been properly installed.
 */
#define IS_WORKING_ERRNO 3333

/**
 * Accessing the SIGSYS siginfo depends on the fields being defined by the libc.
 * Older libc do not yet include the needed definitions and accessor macros.
 * Work around that by falling back to si_value.sival_int which works on some
 * but not all architectures.
 */
#if defined(si_syscall)
# define GET_SYSCALL_NUM(si) ((si)->si_syscall)
#else
#pragma message "your libc doesn't define SIGSYS signal info! \
info about syscall number in case of SECCOMP crash can be invalid"
# define GET_SYSCALL_NUM(si) ((si)->si_value.sival_int)
#endif

static void catchViolation(int sig, siginfo_t* si, void* threadContext)
{
    printf("Attempted banned syscall number [%d] see doc/Seccomp.md for more information\n",
           GET_SYSCALL_NUM(si));

    if (Defined(si_syscall)) {
        printf("Your libc doesn't define SIGSYS signal info. "
               "Above information about syscall number can be invalid.\n");
    }

    Assert_failure("Disallowed Syscall");
}

struct Filter {
    int label;
    int jt;
    int jf;
    struct sock_filter sf;
};

static struct sock_fprog* compile(struct Filter* input, int inputLen, struct Allocator* alloc)
{
    // compute gotos
    int totalOut = 0;
    for (int i = inputLen-1; i >= 0; i--) {
        struct Filter* a = &input[i];
        if (a->label == 0) {
            // check for unresolved gotos...
            Assert_true(a->jt == 0 && a->jf == 0);
            totalOut++;
            continue;
        }
        int diff = 0;
        for (int j = i-1; j >= 0; j--) {
            struct Filter* b = &input[j];
            if (b->label != 0) { continue; }
            if (b->jt == a->label) {
                b->sf.jt = diff;
                b->jt = 0;
            }
            if (b->jf == a->label) {
                b->sf.jf = diff;
                b->jf = 0;
            }
            diff++;
        }
    }

    // copy into output filter array...
    struct sock_filter* sf = Allocator_calloc(alloc, sizeof(struct sock_filter), totalOut);
    int outI = 0;
    for (int i = 0; i < inputLen; i++) {
        if (input[i].label == 0) {
            Bits_memcpy(&sf[outI++], &input[i].sf, sizeof(struct sock_filter));
        }
        Assert_true(outI <= totalOut);
        Assert_true(i != inputLen-1 || outI == totalOut);
    }

    struct sock_fprog* out = Allocator_malloc(alloc, sizeof(struct sock_fprog));
    out->len = (unsigned short) totalOut;
    out->filter = sf;

    return out;
}

#define RET_TRAP      0x00030000u
#define RET_ERRNO(x) (0x00050000u | ((x) & 0x0000ffffu))
#define RET_SUCCESS   0x7fff0000u

static struct sock_fprog* mkFilter(struct Allocator* alloc, struct Except* eh)
{
    // Adding exceptions to the syscall filter:
    //
    // echo '#include <sys/syscall.h>' | gcc -E -dM - | grep 'define __NR_' | sort
    // for the full list of system calls with syscall numbers (different per ABI)
    //
    // If gdb traps out it will look like this:
    //
    //     Program received signal SIGSYS, Bad system call.
    //     [Switching to Thread 0x7ffff7fdd740 (LWP 14673)]
    //     0x00007ffff74d1caa in mmap64 () at ../sysdeps/unix/syscall-template.S:81
    //     81    ../sysdeps/unix/syscall-template.S: No such file or directory.
    //
    // %eax should contain the system call number (on different ABIs YMMV)
    //
    //     (gdb) print $eax
    //     $1 = 9
    //     (gdb)
    //
    // Consult your syscall table from the above gcc command...
    //
    //     #define __NR_mmap 9
    //
    // Then add:
    //
    //     IFEQ(__NR_mmap, success),
    //
    // And add a comment documenting where you needed that syscall :)

    #define STMT(code, val) { .sf = BPF_STMT(code, val) }

    #define JMPK(type, not, input, label) { \
        .sf = BPF_JUMP(BPF_JMP+(type)+BPF_K, (input), 0, 0), \
        .jt = (!(not) ? (label) : 0),                        \
        .jf = ((not) ? (label) : 0)                          \
    }

    // Create a label for jumps, the label must be represented by a non-zero integer.
    #define LABEL(lbl) { .label = (lbl) }

    // Load offset into the register
    #define LOAD(offset)    STMT(BPF_LD+BPF_W+BPF_ABS, (offset))

    // Return constant value
    #define RET(val)        STMT(BPF_RET+BPF_K, (val))

    // If-equal if the currently loaded value equals input, jump to label.
    #define IFEQ(input, label) JMPK(BPF_JEQ, 0, (input), (label))

    // If-not-equal if the currently loaded value is not equal to input, jump to label.
    #define IFNE(input, label) JMPK(BPF_JEQ, 1, (input), (label))

    // If-greater-than
    #define IFGT(input, label) JMPK(BPF_JGT, 0, (input), (label))

    // If-greater-than-or-equal-to
    #define IFGE(input, label) JMPK(BPF_JGE, 0, (input), (label))

    // If-less-than
    #define IFLT(input, label) JMPK(BPF_JGE, 1, (input), (label))

    // If-less-than-or-equal-to
    #define IFLE(input, label) JMPK(BPF_JGT, 1, (input), (label))


    // labels are integers so they must be predefined
    int success = 1;
    int fail = 2;
    int unmaskOnly = 3;
    int isworking = 4;
    int socket = 5;
    int ioctl_setip = 6;
    int bind_netlink = 7;

    uint32_t auditArch = ArchInfo_getAuditArch();

    struct Filter seccompFilter[] = {

        LOAD(offsetof(struct seccomp_data, arch)),
        IFNE(auditArch, fail),

        // Get the syscall num.
        LOAD(offsetof(struct seccomp_data, nr)),

        // udp
        #ifdef __NR_sendmsg
            IFEQ(__NR_sendmsg, success),
        #endif
        #ifdef __NR_recvmsg
            IFEQ(__NR_recvmsg, success),
        #endif

        // ETHInterface
        #ifdef __NR_sendto
            IFEQ(__NR_sendto, success),
        #endif
        #ifdef __NR_recvfrom
            IFEQ(__NR_recvfrom, success),
        #endif

        #ifdef __NR_socketcall
            // 32-bit: recvmsg is a socketcall
            IFEQ(__NR_socketcall, success),
        #endif

        // libuv
        IFEQ(__NR_epoll_ctl, success),
        #ifdef __NR_epoll_wait
            IFEQ(__NR_epoll_wait, success),
        #endif
        #ifdef __NR_epoll_pwait
            IFEQ(__NR_epoll_pwait, success),
        #endif

        // gettimeofday is required on some architectures
        #ifdef __NR_gettimeofday
            IFEQ(__NR_gettimeofday, success),
        #endif

        // TUN (and logging)
        IFEQ(__NR_write, success),
        IFEQ(__NR_read, success),
        // readv and writev are used by some libc (musl)
        #ifdef __NR_readv
            IFEQ(__NR_readv, success),
        #endif
        #ifdef __NR_writev
            IFEQ(__NR_writev, success),
        #endif

        // modern librt reads a read-only mapped section of kernel space which contains the time
        // older versions need system calls for getting the time.
        // i686 glibc-2.18's time() uses __NR_time
        // Raspberry Pi and BeagleBone Black don't provide __NR_time
        IFEQ(__NR_clock_gettime, success),
        #ifdef __NR_time
            IFEQ(__NR_time, success),
        #endif

        // NetPlatform_linux.c send recv
        #ifdef __NR_send
            IFEQ(__NR_send, success),
        #endif
        #ifdef __NR_recv
            IFEQ(__NR_recv, success),
        #endif

        // malloc()
        IFEQ(__NR_brk, success),

        // abort()
        IFEQ(__NR_gettid, success),
        IFEQ(__NR_tgkill, success),
        IFEQ(__NR_rt_sigprocmask, unmaskOnly),

        // exit()
        IFEQ(__NR_exit_group, success),

        // Seccomp_isWorking()
        IFEQ(__NR_getpriority, isworking),

        // Securiy_checkPermissions() -> canOpenFiles()
        IFEQ(__NR_dup, success),
        IFEQ(__NR_close, success),

        // Security_checkPermissions() -> getMaxMem()
        // x86/ARM use ugetrlimit and mmap2
        // ARM does not even have __NR_getrlimit or __NR_mmap defined
        // and AMD64 does not have __NR_ugetrlimit or __NR_mmap2 defined
        #ifdef __NR_getrlimit
            IFEQ(__NR_getrlimit, success),
        #endif
        #ifdef __NR_ugetrlimit
            IFEQ(__NR_ugetrlimit, success),
        #endif
        #ifdef __NR_mmap
            IFEQ(__NR_mmap, success),
        #endif
        #ifdef __NR_mmap2
            IFEQ(__NR_mmap2, success),
        #endif
        IFEQ(__NR_munmap, success),

        // printf()
        IFEQ(__NR_fstat, success),
        #ifdef __NR_fstat64
            IFEQ(__NR_fstat64, success),
        #endif

        // for setting IP addresses
        // socketForIfName()
        // and ETHInterface_listDevices
        #ifdef __NR_socket
            IFEQ(__NR_socket, socket),
        #endif
        IFEQ(__NR_ioctl, ioctl_setip),

        // Security_checkPermissions
        IFEQ(__NR_getuid, success),
        // Security_nofiles
        IFEQ(__NR_setrlimit, success),

        // for ETHInterface_listDevices (netlinkk)
        #ifdef __NR_bind
        IFEQ(__NR_bind, bind_netlink),
        #endif
        #ifdef __NR_getsockname
        IFEQ(__NR_getsockname, success),
        #endif
        RET(SECCOMP_RET_TRAP),

        LABEL(socket),
        LOAD(offsetof(struct seccomp_data, args[1])),
        IFEQ(SOCK_DGRAM, success),
        LOAD(offsetof(struct seccomp_data, args[0])),
        IFEQ(AF_NETLINK, success),
        RET(SECCOMP_RET_TRAP),

        LABEL(ioctl_setip),
        LOAD(offsetof(struct seccomp_data, args[1])),
        IFEQ(SIOCGIFINDEX, success),
        IFEQ(SIOCGIFFLAGS, success),
        IFEQ(SIOCSIFFLAGS, success),
        IFEQ(SIOCSIFADDR, success),
        IFEQ(SIOCSIFNETMASK, success),
        IFEQ(SIOCSIFMTU, success),
        RET(SECCOMP_RET_TRAP),

        LABEL(bind_netlink),
        LOAD(offsetof(struct seccomp_data, args[2])),
        // Filter NETLINK by size of address.
        // Most importantly INET and INET6
        // are differnt.
        IFEQ(sizeof(struct sockaddr_nl), success),
        RET(SECCOMP_RET_TRAP),

        // We allow sigprocmask to *unmask* signals but we don't allow it to mask them.
        LABEL(unmaskOnly),
        LOAD(offsetof(struct seccomp_data, args[0])),
        IFEQ(SIG_UNBLOCK, success),
        RET(SECCOMP_RET_TRAP),

        LABEL(isworking),
        RET(RET_ERRNO(IS_WORKING_ERRNO)),

        LABEL(fail),
        RET(SECCOMP_RET_TRAP),

        LABEL(success),
        RET(SECCOMP_RET_ALLOW),
    };

    return compile(seccompFilter, sizeof(seccompFilter)/sizeof(seccompFilter[0]), alloc);
}

static void installFilter(struct sock_fprog* filter, struct Log* logger, struct Except* eh)
{
    struct sigaction sa = { .sa_sigaction = catchViolation, .sa_flags = SA_SIGINFO };
    if (sigaction(SIGSYS, &sa, NULL)) {
        Log_warn(logger, "sigaction(SIGSYS) -> [%s]\n", strerror(errno));
    }

    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) == -1) {
        // don't worry about it.
        Log_warn(logger, "prctl(PR_SET_NO_NEW_PRIVS) -> [%s]\n", strerror(errno));
    }
    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, filter) == -1) {
        Except_throw(eh, "prctl(PR_SET_SECCOMP) -> [%s]\n", strerror(errno));
    }
}

void Seccomp_dropPermissions(struct Allocator* tempAlloc, struct Log* logger, struct Except* eh)
{
    struct sock_fprog* filter = mkFilter(tempAlloc, eh);
    installFilter(filter, logger, eh);
    if (!Seccomp_isWorking()) {
        Except_throw(eh, "Seccomp filter not installed properly, Seccomp_isWorking() -> false");
    }
}

int Seccomp_isWorking()
{
    errno = 0;
    // If seccomp is not working, this will fail setting errno to EINVAL
    long ret = getpriority(1000, 1);

    int err = errno;

    // Inside of the kernel, it seems to check whether the errno return is sane
    // and if it is not, it treates it as a return value, IS_WORKING_ERRNO (3333) is very unique so
    // we'll check for either case just in case this changes.
    return (ret == -1 && err == IS_WORKING_ERRNO) || (ret == -IS_WORKING_ERRNO && err == 0);
}

int Seccomp_exists()
{
    return 1;
}
