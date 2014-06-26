# Seccomp

SECCOMP (secure computing) is a way for programs to declare to the Linux kernel
that they will never make certain system calls, thus any attempt to make one of
these calls is interpreted as a security penetration and the kernel can forcibly
kill off the program, preventing harm to the computer.

## Seccomp failures in cjdns

If you are reading this because cjdns is halting on you, you are probably getting
a log message like the following:

        Attempted banned syscall number [232] see docs/Seccomp.md for more information

This number (`232` in the example) is specific to your system and you need to
run a command to convert it to a syscall name.

        echo '#include <sys/syscall.h>' | cpp -dM | grep '#define __NR_.* 232'

Obviously you'll be replacing `232` with the actual syscall number which your system
printed. The Result might look something like the following:

        #define __NR_epoll_wait 232

Which would tell you (for example) that the `epoll_wait` syscall was disallowed on
your system. In this case you'd need to go to `util/Seccomp.c` and inside of the
`mkfilter()` function where the actual SECCOMP rules are set up, you'll see a set
of entries such as the following.

        #ifdef __NR_mmap2
            IFEQ(__NR_mmap2, success),
        #endif

Add a similar entry for the syscall (make sure you put it with the others and not)
below the `RET(SECCOMP_RET_TRAP),` line which triggers the failure). When you have
finished adding your system call rebuild and re-test cjdns. If it works well then
please make a Pull Request :-)   If not then open a bug report and explain the problem.
