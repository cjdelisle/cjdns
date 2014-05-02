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
#include "util/Seccomp.h"

// SIG_UNBLOCK
#define _POSIX_SOURCE
#include <signal.h>

// getpriority()
#include <sys/resource.h>
#include <errno.h>
#include <seccomp.h>
#include <stddef.h>

void Seccomp_dropPermissions(struct Except* eh)
{
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_TRAP);
    if (!ctx) {
        seccomp_release(ctx);
        Except_throw(eh, "Failed to initialize SECCOMP");
    }

    int rc = 0;

    // Adding exceptions to the syscall filter:
    //
    // echo '#include <seccomp.h>' | gcc -E -dM - | grep 'define __NR_' | sort
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
    //     rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap), 0);
    //
    // And add a comment documenting where you needed that syscall :)

    // see seccomp.h:307 and "man seccomp_rule_add" for docs on this function


    // libuv
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(epoll_ctl), 0);
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(epoll_wait), 0);

    // logging to stdout/stderr, tun and eth0
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0);

    // tun device and eth0
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0);

    // malloc()
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(brk), 0);

    // udp
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sendmsg), 0);
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(recvmsg), 0);

    // ETHInterface
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sendto), 0);
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(recvfrom), 0);

    // abort()
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(gettid), 0);
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(tgkill), 0);
    rc |= seccomp_rule_add(ctx,
                           SCMP_ACT_ALLOW,
                           SCMP_SYS(rt_sigprocmask),
                           1,
                           SCMP_CMP(0, SCMP_CMP_EQ, SIG_UNBLOCK, 0));

    // exit()
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0);

    // Seccomp_isWorking()
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ERRNO(9000), SCMP_SYS(getpriority), 0);

    // Securiy_checkPermissions() -> canOpenFiles()
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(dup), 0);
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);

    // Security_checkPermissions() -> getMaxMem()
    // x86 uses ugetrlimit and mmap2
    #ifdef __NR_getrlimit
        rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(getrlimit), 0);
    #endif
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(ugetrlimit), 0);
    #ifdef __NR_mmap
        rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap), 0);
    #endif
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(mmap2), 0);

    // Seem to be used on i686 (Linux 3.12.18 SMP PREEMPT i686 GNU/Linux) from glibc-2.18's time()
    #ifdef __NR_time
        rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(time), 0);
    #endif

    // printf
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(fstat), 0);

    // Some machines need this
    rc |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(clock_gettime), 0);

    /////

    char* err = NULL;
    if (rc) {
        err = "Failed to add SECCOMP filters";
    } else if (seccomp_load(ctx)) {
        err = "Failed to load SECCOMP";
    }

    seccomp_release(ctx);

    if (err) {
        Except_throw(eh, "%s", err);
    } else if (!Seccomp_isWorking()) {
        Except_throw(eh, "SECCOMP is not working correctly");
    }
}

int Seccomp_isWorking()
{
    errno = 0;
    // If seccomp is not working, this will fail setting errno to EINVAL
    long ret = getpriority(1000, 1);

    // Inside of the kernel, it seems to check whether the errno return is sane
    // and if it is not, it treates it as a return value, 9000 is very unique so
    // we'll check for either case just in case this changes.
    return (ret == -1 && errno == 9000) || (ret == -9000 && errno == 0);
}
