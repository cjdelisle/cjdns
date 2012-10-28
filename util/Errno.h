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
#ifndef Errno_H
#define Errno_H


#include <errno.h>
#ifdef WIN32
#include <winsock.h>
#elif defined(__FreeBSD__) || defined(__APPLE__)
#include <sys/errno.h>
#endif

#define Errno_EMULTIHOP EMULTIHOP
#define Errno_EUNATCH EUNATCH
#define Errno_EAFNOSUPPORT EAFNOSUPPORT
#define Errno_EREMCHG EREMCHG
#define Errno_EACCES EACCES
#define Errno_EDESTADDRREQ EDESTADDRREQ
#define Errno_EILSEQ EILSEQ
#define Errno_ESPIPE ESPIPE
#define Errno_EMLINK EMLINK
#define Errno_EOWNERDEAD EOWNERDEAD
#define Errno_ENOTTY ENOTTY
#define Errno_EBADE EBADE
#define Errno_EBADF EBADF
#define Errno_EBADR EBADR
#define Errno_EADV EADV
#define Errno_ERANGE ERANGE
#define Errno_ECANCELED ECANCELED
#define Errno_ETXTBSY ETXTBSY
#define Errno_ENOMEM ENOMEM
#define Errno_EINPROGRESS EINPROGRESS
#define Errno_ENOTBLK ENOTBLK
#define Errno_EPROTOTYPE EPROTOTYPE
#define Errno_ERESTART ERESTART
#define Errno_EISNAM EISNAM
#define Errno_ENOMSG ENOMSG
#define Errno_EALREADY EALREADY
#define Errno_ETIMEDOUT ETIMEDOUT
#define Errno_ENODATA ENODATA
#define Errno_EINTR EINTR
#define Errno_ENOLINK ENOLINK
#define Errno_EPERM EPERM
#define Errno_ELOOP ELOOP
#define Errno_ENETDOWN ENETDOWN
#define Errno_ESTALE ESTALE
#define Errno_ENOSR ENOSR
#define Errno_ELNRNG ELNRNG
#define Errno_EPIPE EPIPE
#define Errno_ECHILD ECHILD
#define Errno_EBADMSG EBADMSG
#define Errno_EBFONT EBFONT
#define Errno_EREMOTE EREMOTE
#define Errno_ETOOMANYREFS ETOOMANYREFS
#define Errno_ENONET ENONET
#define Errno_EXFULL EXFULL
#define Errno_ENOTEMPTY ENOTEMPTY
#define Errno_ENOTNAM ENOTNAM
#define Errno_ENOCSI ENOCSI
#define Errno_EADDRINUSE EADDRINUSE
#define Errno_ENETRESET ENETRESET
#define Errno_EISDIR EISDIR
#define Errno_EIDRM EIDRM
#define Errno_ENOTSOCK ENOTSOCK
#define Errno_EBADFD EBADFD
#define Errno_EL3HLT EL3HLT
#define Errno_EL2HLT EL2HLT
#define Errno_ENOKEY ENOKEY
#define Errno_EINVAL EINVAL
#define Errno_ESHUTDOWN ESHUTDOWN
#define Errno_EKEYREJECTED EKEYREJECTED
#define Errno_ENOMEDIUM ENOMEDIUM
#define Errno_ELIBSCN ELIBSCN
#define Errno_ENAVAIL ENAVAIL
#define Errno_EOVERFLOW EOVERFLOW
#define Errno_EUCLEAN EUCLEAN
#define Errno_EBUSY EBUSY
#define Errno_EPROTO EPROTO
#define Errno_ENODEV ENODEV
#define Errno_EKEYEXPIRED EKEYEXPIRED
#define Errno_EROFS EROFS
#define Errno_ELIBACC ELIBACC
#define Errno_EHWPOISON EHWPOISON
#define Errno_E2BIG E2BIG
#define Errno_EL3RST EL3RST
#define Errno_ENOTDIR ENOTDIR
#define Errno_ECONNRESET ECONNRESET
#define Errno_ENXIO ENXIO
#define Errno_EBADRQC EBADRQC
#define Errno_ENOSTR ENOSTR
#define Errno_ENAMETOOLONG ENAMETOOLONG
#define Errno_ESOCKTNOSUPPORT ESOCKTNOSUPPORT
#define Errno_ELIBEXEC ELIBEXEC
#define Errno_EDOTDOT EDOTDOT
#define Errno_EADDRNOTAVAIL EADDRNOTAVAIL
#define Errno_ETIME ETIME
#define Errno_EPROTONOSUPPORT EPROTONOSUPPORT
#define Errno_ENOTRECOVERABLE ENOTRECOVERABLE
#define Errno_EIO EIO
#define Errno_ENETUNREACH ENETUNREACH
#define Errno_EXDEV EXDEV
#define Errno_EDQUOT EDQUOT
#define Errno_EREMOTEIO EREMOTEIO
#define Errno_ENOSPC ENOSPC
#define Errno_ENOEXEC ENOEXEC
#ifdef WIN32
 #define Errno_EMSGSIZE WSAEMSGSIZE
#else
 #define Errno_EMSGSIZE EMSGSIZE
#endif
#define Errno_EDOM EDOM
#define Errno_EFBIG EFBIG
#define Errno_ESRCH ESRCH
#define Errno_ECHRNG ECHRNG
#define Errno_EHOSTDOWN EHOSTDOWN
#define Errno_ENOLCK ENOLCK
#define Errno_ENFILE ENFILE
#define Errno_ENOSYS ENOSYS
#define Errno_ENOTCONN ENOTCONN
#define Errno_EPFNOSUPPORT EPFNOSUPPORT
#define Errno_ENOTSUP ENOTSUP
#define Errno_ESRMNT ESRMNT
#define Errno_EDEADLOCK EDEADLOCK
#define Errno_ECONNABORTED ECONNABORTED
#define Errno_ENOANO ENOANO
#define Errno_EISCONN EISCONN
#define Errno_EUSERS EUSERS
#define Errno_ENOPROTOOPT ENOPROTOOPT
#define Errno_ECOMM ECOMM
#define Errno_EMFILE EMFILE
#define Errno_ERFKILL ERFKILL
#ifdef WIN32
 #define Errno_ENOBUFS WSAENOBUFS
#else
 #define Errno_ENOBUFS ENOBUFS
#endif
#define Errno_EFAULT EFAULT
#define Errno_EWOULDBLOCK EWOULDBLOCK
#define Errno_ELIBBAD ELIBBAD
#define Errno_ESTRPIPE ESTRPIPE
#define Errno_ECONNREFUSED ECONNREFUSED
#define Errno_EAGAIN EAGAIN
#define Errno_ELIBMAX ELIBMAX
#define Errno_EEXIST EEXIST
#define Errno_EL2NSYNC EL2NSYNC
#define Errno_ENOENT ENOENT
#define Errno_ENOPKG ENOPKG
#define Errno_EBADSLT EBADSLT
#define Errno_EKEYREVOKED EKEYREVOKED
#define Errno_EHOSTUNREACH EHOSTUNREACH
#define Errno_ENOTUNIQ ENOTUNIQ
#define Errno_EOPNOTSUPP EOPNOTSUPP
#define Errno_EMEDIUMTYPE EMEDIUMTYPE

int Errno_get();

void Errno_clear();

#endif
