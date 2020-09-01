#include <linux/audit.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <sys/prctl.h>
#include <sys/resource.h>
#include <sys/syscall.h>

int main (void)
{
	return __NR_read | PR_SET_NO_NEW_PRIVS | PR_SET_SECCOMP |
		AUDIT_ARCH_X86_64 | BPF_K | SECCOMP_MODE_FILTER;
}

