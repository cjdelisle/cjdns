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
#define Errno_NO_MAPPINGS
#include "util/Errno.h"


#include <errno.h>
#ifdef Windows
    #include <winsock.h>
#elif defined(FreeBSD) || defined(Darwin)
    #include <sys/errno.h>
#endif

static inline enum Errno checkMask(enum Errno err, uint64_t mappingMask)
{
    switch (err) {
    #ifdef ENOTSUP
        case Errno_ENOTSUP:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_ENOTSUP))
                ? Errno_ENOTSUP : Errno_EOPNOTSUPP;
    #endif
    #ifdef EWOULDBLOCK
        case Errno_EWOULDBLOCK:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_EWOULDBLOCK))
                ? Errno_EWOULDBLOCK : Errno_EAGAIN;
    #endif
    #ifdef EDEADLK
        case Errno_EDEADLK:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_EDEADLK))
                ? Errno_EDEADLK : Errno_EDEADLOCK;
    #endif
    #ifdef WSAEWOULDBLOCK
        case Errno_WSAEWOULDBLOCK:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEWOULDBLOCK))
                ? Errno_WSAEWOULDBLOCK : Errno_EWOULDBLOCK;
    #endif
    #ifdef WSAEINPROGRESS
        case Errno_WSAEINPROGRESS:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEINPROGRESS))
                ? Errno_WSAEINPROGRESS : Errno_EINPROGRESS;
    #endif
    #ifdef WSAEALREADY
        case Errno_WSAEALREADY:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEALREADY))
                ? Errno_WSAEALREADY : Errno_EALREADY;
    #endif
    #ifdef WSAENOTSOCK
        case Errno_WSAENOTSOCK:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENOTSOCK))
                ? Errno_WSAENOTSOCK : Errno_ENOTSOCK;
    #endif
    #ifdef WSAEDESTADDRREQ
        case Errno_WSAEDESTADDRREQ:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEDESTADDRREQ))
                ? Errno_WSAEDESTADDRREQ : Errno_EDESTADDRREQ;
    #endif
    #ifdef WSAEMSGSIZE
        case Errno_WSAEMSGSIZE:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEMSGSIZE))
                ? Errno_WSAEMSGSIZE : Errno_EMSGSIZE;
    #endif
    #ifdef WSAEPROTOTYPE
        case Errno_WSAEPROTOTYPE:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEPROTOTYPE))
                ? Errno_WSAEPROTOTYPE : Errno_EPROTOTYPE;
    #endif
    #ifdef WSAENOPROTOOPT
        case Errno_WSAENOPROTOOPT:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENOPROTOOPT))
                ? Errno_WSAENOPROTOOPT : Errno_ENOPROTOOPT;
    #endif
    #ifdef WSAEPROTONOSUPPORT
        case Errno_WSAEPROTONOSUPPORT:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEPROTONOSUPPORT))
                ? Errno_WSAEPROTONOSUPPORT : Errno_EPROTONOSUPPORT;
    #endif
    #ifdef WSAESOCKTNOSUPPORT
        case Errno_WSAESOCKTNOSUPPORT:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAESOCKTNOSUPPORT))
                ? Errno_WSAESOCKTNOSUPPORT : Errno_ESOCKTNOSUPPORT;
    #endif
    #ifdef WSAEOPNOTSUPP
        case Errno_WSAEOPNOTSUPP:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEOPNOTSUPP))
                ? Errno_WSAEOPNOTSUPP : Errno_EOPNOTSUPP;
    #endif
    #ifdef WSAEPFNOSUPPORT
        case Errno_WSAEPFNOSUPPORT:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEPFNOSUPPORT))
                ? Errno_WSAEPFNOSUPPORT : Errno_EPFNOSUPPORT;
    #endif
    #ifdef WSAEAFNOSUPPORT
        case Errno_WSAEAFNOSUPPORT:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEAFNOSUPPORT))
                ? Errno_WSAEAFNOSUPPORT : Errno_EAFNOSUPPORT;
    #endif
    #ifdef WSAEADDRINUSE
        case Errno_WSAEADDRINUSE:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEADDRINUSE))
                ? Errno_WSAEADDRINUSE : Errno_EADDRINUSE;
    #endif
    #ifdef WSAEADDRNOTAVAIL
        case Errno_WSAEADDRNOTAVAIL:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEADDRNOTAVAIL))
                ? Errno_WSAEADDRNOTAVAIL : Errno_EADDRNOTAVAIL;
    #endif
    #ifdef WSAENETDOWN
        case Errno_WSAENETDOWN:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENETDOWN))
                ? Errno_WSAENETDOWN : Errno_ENETDOWN;
    #endif
    #ifdef WSAENETUNREACH
        case Errno_WSAENETUNREACH:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENETUNREACH))
                ? Errno_WSAENETUNREACH : Errno_ENETUNREACH;
    #endif
    #ifdef WSAENETRESET
        case Errno_WSAENETRESET:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENETRESET))
                ? Errno_WSAENETRESET : Errno_ENETRESET;
    #endif
    #ifdef WSAECONNABORTED
        case Errno_WSAECONNABORTED:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAECONNABORTED))
                ? Errno_WSAECONNABORTED : Errno_ECONNABORTED;
    #endif
    #ifdef WSAECONNRESET
        case Errno_WSAECONNRESET:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAECONNRESET))
                ? Errno_WSAECONNRESET : Errno_ECONNRESET;
    #endif
    #ifdef WSAENOBUFS
        case Errno_WSAENOBUFS:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENOBUFS))
                ? Errno_WSAENOBUFS : Errno_ENOBUFS;
    #endif
    #ifdef WSAEISCONN
        case Errno_WSAEISCONN:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEISCONN))
                ? Errno_WSAEISCONN : Errno_EISCONN;
    #endif
    #ifdef WSAENOTCONN
        case Errno_WSAENOTCONN:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENOTCONN))
                ? Errno_WSAENOTCONN : Errno_ENOTCONN;
    #endif
    #ifdef WSAESHUTDOWN
        case Errno_WSAESHUTDOWN:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAESHUTDOWN))
                ? Errno_WSAESHUTDOWN : Errno_ESHUTDOWN;
    #endif
    #ifdef WSAETOOMANYREFS
        case Errno_WSAETOOMANYREFS:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAETOOMANYREFS))
                ? Errno_WSAETOOMANYREFS : Errno_ETOOMANYREFS;
    #endif
    #ifdef WSAETIMEDOUT
        case Errno_WSAETIMEDOUT:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAETIMEDOUT))
                ? Errno_WSAETIMEDOUT : Errno_ETIMEDOUT;
    #endif
    #ifdef WSAECONNREFUSED
        case Errno_WSAECONNREFUSED:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAECONNREFUSED))
                ? Errno_WSAECONNREFUSED : Errno_ECONNREFUSED;
    #endif
    #ifdef WSAELOOP
        case Errno_WSAELOOP:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAELOOP))
                ? Errno_WSAELOOP : Errno_ELOOP;
    #endif
    #ifdef WSAENAMETOOLONG
        case Errno_WSAENAMETOOLONG:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENAMETOOLONG))
                ? Errno_WSAENAMETOOLONG : Errno_ENAMETOOLONG;
    #endif
    #ifdef WSAEHOSTDOWN
        case Errno_WSAEHOSTDOWN:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEHOSTDOWN))
                ? Errno_WSAEHOSTDOWN : Errno_EHOSTDOWN;
    #endif
    #ifdef WSAEHOSTUNREACH
        case Errno_WSAEHOSTUNREACH:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEHOSTUNREACH))
                ? Errno_WSAEHOSTUNREACH : Errno_EHOSTUNREACH;
    #endif
    #ifdef WSAENOTEMPTY
        case Errno_WSAENOTEMPTY:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAENOTEMPTY))
                ? Errno_WSAENOTEMPTY : Errno_ENOTEMPTY;
    #endif
    #ifdef WSAEPROCLIM
        case Errno_WSAEPROCLIM:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEPROCLIM))
                ? Errno_WSAEPROCLIM : Errno_EPROCLIM;
    #endif
    #ifdef WSAEUSERS
        case Errno_WSAEUSERS:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEUSERS))
                ? Errno_WSAEUSERS : Errno_EUSERS;
    #endif
    #ifdef WSAEDQUOT
        case Errno_WSAEDQUOT:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEDQUOT))
                ? Errno_WSAEDQUOT : Errno_EDQUOT;
    #endif
    #ifdef WSAESTALE
        case Errno_WSAESTALE:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAESTALE))
                ? Errno_WSAESTALE : Errno_ESTALE;
    #endif
    #ifdef WSAEREMOTE
        case Errno_WSAEREMOTE:
            return (mappingMask & (((uint64_t)1) << Errno_Mappings_WSAEREMOTE))
                ? Errno_WSAEREMOTE : Errno_EREMOTE;
    #endif
        default: return err;
    }
}

enum Errno Errno_getInternal(uint64_t mappingMask)
{
    #ifdef Windows
        int err = WSAGetLastError();
    #else
        int err = errno;
    #endif

    switch (err) {
        case 0:
            return Errno_NO_ERROR;
        #ifdef E2BIG
            case E2BIG:
                return checkMask(Errno_E2BIG, mappingMask);
        #endif
        #ifdef EACCES
            case EACCES:
                return checkMask(Errno_EACCES, mappingMask);
        #endif
        #ifdef EADDRINUSE
            case EADDRINUSE:
                return checkMask(Errno_EADDRINUSE, mappingMask);
        #endif
        #ifdef EADDRNOTAVAIL
            case EADDRNOTAVAIL:
                return checkMask(Errno_EADDRNOTAVAIL, mappingMask);
        #endif
        #ifdef EADV
            case EADV:
                return checkMask(Errno_EADV, mappingMask);
        #endif
        #ifdef EAFNOSUPPORT
            case EAFNOSUPPORT:
                return checkMask(Errno_EAFNOSUPPORT, mappingMask);
        #endif
        #ifdef EAGAIN
            case EAGAIN:
                return checkMask(Errno_EAGAIN, mappingMask);
        #endif
        #ifdef EALREADY
            case EALREADY:
                return checkMask(Errno_EALREADY, mappingMask);
        #endif
        #ifdef EAUTH
            case EAUTH:
                return checkMask(Errno_EAUTH, mappingMask);
        #endif
        #ifdef EBADE
            case EBADE:
                return checkMask(Errno_EBADE, mappingMask);
        #endif
        #ifdef EBADF
            case EBADF:
                return checkMask(Errno_EBADF, mappingMask);
        #endif
        #ifdef EBADFD
            case EBADFD:
                return checkMask(Errno_EBADFD, mappingMask);
        #endif
        #ifdef EBADMSG
            case EBADMSG:
                return checkMask(Errno_EBADMSG, mappingMask);
        #endif
        #ifdef EBADR
            case EBADR:
                return checkMask(Errno_EBADR, mappingMask);
        #endif
        #ifdef EBADRPC
            case EBADRPC:
                return checkMask(Errno_EBADRPC, mappingMask);
        #endif
        #ifdef EBADRQC
            case EBADRQC:
                return checkMask(Errno_EBADRQC, mappingMask);
        #endif
        #ifdef EBADSLT
            case EBADSLT:
                return checkMask(Errno_EBADSLT, mappingMask);
        #endif
        #ifdef EBFONT
            case EBFONT:
                return checkMask(Errno_EBFONT, mappingMask);
        #endif
        #ifdef EBUSY
            case EBUSY:
                return checkMask(Errno_EBUSY, mappingMask);
        #endif
        #ifdef ECANCELED
            case ECANCELED:
                return checkMask(Errno_ECANCELED, mappingMask);
        #endif
        #ifdef ECHILD
            case ECHILD:
                return checkMask(Errno_ECHILD, mappingMask);
        #endif
        #ifdef ECHRNG
            case ECHRNG:
                return checkMask(Errno_ECHRNG, mappingMask);
        #endif
        #ifdef ECOMM
            case ECOMM:
                return checkMask(Errno_ECOMM, mappingMask);
        #endif
        #ifdef ECONNABORTED
            case ECONNABORTED:
                return checkMask(Errno_ECONNABORTED, mappingMask);
        #endif
        #ifdef ECONNREFUSED
            case ECONNREFUSED:
                return checkMask(Errno_ECONNREFUSED, mappingMask);
        #endif
        #ifdef ECONNRESET
            case ECONNRESET:
                return checkMask(Errno_ECONNRESET, mappingMask);
        #endif
        #ifdef EDEADLK
            case EDEADLK:
                return checkMask(Errno_EDEADLK, mappingMask);
        #endif
        #if defined(EDEADLOCK) && EDEADLOCK != EDEADLK
            case EDEADLOCK:
                return checkMask(Errno_EDEADLOCK, mappingMask);
        #endif
        #ifdef EDESTADDRREQ
            case EDESTADDRREQ:
                return checkMask(Errno_EDESTADDRREQ, mappingMask);
        #endif
        #ifdef EDOM
            case EDOM:
                return checkMask(Errno_EDOM, mappingMask);
        #endif
        #ifdef EDOTDOT
            case EDOTDOT:
                return checkMask(Errno_EDOTDOT, mappingMask);
        #endif
        #ifdef EDQUOT
            case EDQUOT:
                return checkMask(Errno_EDQUOT, mappingMask);
        #endif
        #ifdef EEXIST
            case EEXIST:
                return checkMask(Errno_EEXIST, mappingMask);
        #endif
        #ifdef EFAULT
            case EFAULT:
                return checkMask(Errno_EFAULT, mappingMask);
        #endif
        #ifdef EFBIG
            case EFBIG:
                return checkMask(Errno_EFBIG, mappingMask);
        #endif
        #ifdef EFTYPE
            case EFTYPE:
                return checkMask(Errno_EFTYPE, mappingMask);
        #endif
        #ifdef EHOSTDOWN
            case EHOSTDOWN:
                return checkMask(Errno_EHOSTDOWN, mappingMask);
        #endif
        #ifdef EHOSTUNREACH
            case EHOSTUNREACH:
                return checkMask(Errno_EHOSTUNREACH, mappingMask);
        #endif
        #ifdef EIDRM
            case EIDRM:
                return checkMask(Errno_EIDRM, mappingMask);
        #endif
        #ifdef EILSEQ
            case EILSEQ:
                return checkMask(Errno_EILSEQ, mappingMask);
        #endif
        #ifdef EINPROGRESS
            case EINPROGRESS:
                return checkMask(Errno_EINPROGRESS, mappingMask);
        #endif
        #ifdef EINTR
            case EINTR:
                return checkMask(Errno_EINTR, mappingMask);
        #endif
        #ifdef EINVAL
            case EINVAL:
                return checkMask(Errno_EINVAL, mappingMask);
        #endif
        #ifdef EIO
            case EIO:
                return checkMask(Errno_EIO, mappingMask);
        #endif
        #ifdef EISCONN
            case EISCONN:
                return checkMask(Errno_EISCONN, mappingMask);
        #endif
        #ifdef EISDIR
            case EISDIR:
                return checkMask(Errno_EISDIR, mappingMask);
        #endif
        #ifdef EISNAM
            case EISNAM:
                return checkMask(Errno_EISNAM, mappingMask);
        #endif
        #ifdef EKEYEXPIRED
            case EKEYEXPIRED:
                return checkMask(Errno_EKEYEXPIRED, mappingMask);
        #endif
        #ifdef EKEYREJECTED
            case EKEYREJECTED:
                return checkMask(Errno_EKEYREJECTED, mappingMask);
        #endif
        #ifdef EKEYREVOKED
            case EKEYREVOKED:
                return checkMask(Errno_EKEYREVOKED, mappingMask);
        #endif
        #ifdef EL2HLT
            case EL2HLT:
                return checkMask(Errno_EL2HLT, mappingMask);
        #endif
        #ifdef EL2NSYNC
            case EL2NSYNC:
                return checkMask(Errno_EL2NSYNC, mappingMask);
        #endif
        #ifdef EL3HLT
            case EL3HLT:
                return checkMask(Errno_EL3HLT, mappingMask);
        #endif
        #ifdef EL3RST
            case EL3RST:
                return checkMask(Errno_EL3RST, mappingMask);
        #endif
        #ifdef ELIBACC
            case ELIBACC:
                return checkMask(Errno_ELIBACC, mappingMask);
        #endif
        #ifdef ELIBBAD
            case ELIBBAD:
                return checkMask(Errno_ELIBBAD, mappingMask);
        #endif
        #ifdef ELIBEXEC
            case ELIBEXEC:
                return checkMask(Errno_ELIBEXEC, mappingMask);
        #endif
        #ifdef ELIBMAX
            case ELIBMAX:
                return checkMask(Errno_ELIBMAX, mappingMask);
        #endif
        #ifdef ELIBSCN
            case ELIBSCN:
                return checkMask(Errno_ELIBSCN, mappingMask);
        #endif
        #ifdef ELNRNG
            case ELNRNG:
                return checkMask(Errno_ELNRNG, mappingMask);
        #endif
        #ifdef ELOOP
            case ELOOP:
                return checkMask(Errno_ELOOP, mappingMask);
        #endif
        #ifdef EMEDIUMTYPE
            case EMEDIUMTYPE:
                return checkMask(Errno_EMEDIUMTYPE, mappingMask);
        #endif
        #ifdef EMFILE
            case EMFILE:
                return checkMask(Errno_EMFILE, mappingMask);
        #endif
        #ifdef EMLINK
            case EMLINK:
                return checkMask(Errno_EMLINK, mappingMask);
        #endif
        #ifdef EMSGSIZE
            case EMSGSIZE:
                return checkMask(Errno_EMSGSIZE, mappingMask);
        #endif
        #ifdef EMULTIHOP
            case EMULTIHOP:
                return checkMask(Errno_EMULTIHOP, mappingMask);
        #endif
        #ifdef ENAMETOOLONG
            case ENAMETOOLONG:
                return checkMask(Errno_ENAMETOOLONG, mappingMask);
        #endif
        #ifdef ENAVAIL
            case ENAVAIL:
                return checkMask(Errno_ENAVAIL, mappingMask);
        #endif
        #ifdef ENEEDAUTH
            case ENEEDAUTH:
                return checkMask(Errno_ENEEDAUTH, mappingMask);
        #endif
        #ifdef ENETDOWN
            case ENETDOWN:
                return checkMask(Errno_ENETDOWN, mappingMask);
        #endif
        #ifdef ENETRESET
            case ENETRESET:
                return checkMask(Errno_ENETRESET, mappingMask);
        #endif
        #ifdef ENETUNREACH
            case ENETUNREACH:
                return checkMask(Errno_ENETUNREACH, mappingMask);
        #endif
        #ifdef ENFILE
            case ENFILE:
                return checkMask(Errno_ENFILE, mappingMask);
        #endif
        #ifdef ENOANO
            case ENOANO:
                return checkMask(Errno_ENOANO, mappingMask);
        #endif
        #ifdef ENOATTR
            case ENOATTR:
                return checkMask(Errno_ENOATTR, mappingMask);
        #endif
        #ifdef ENOBUFS
            case ENOBUFS:
                return checkMask(Errno_ENOBUFS, mappingMask);
        #endif
        #ifdef ENOCSI
            case ENOCSI:
                return checkMask(Errno_ENOCSI, mappingMask);
        #endif
        #ifdef ENODATA
            case ENODATA:
                return checkMask(Errno_ENODATA, mappingMask);
        #endif
        #ifdef ENODEV
            case ENODEV:
                return checkMask(Errno_ENODEV, mappingMask);
        #endif
        #ifdef ENOENT
            case ENOENT:
                return checkMask(Errno_ENOENT, mappingMask);
        #endif
        #ifdef ENOEXEC
            case ENOEXEC:
                return checkMask(Errno_ENOEXEC, mappingMask);
        #endif
        #ifdef ENOKEY
            case ENOKEY:
                return checkMask(Errno_ENOKEY, mappingMask);
        #endif
        #ifdef ENOLCK
            case ENOLCK:
                return checkMask(Errno_ENOLCK, mappingMask);
        #endif
        #ifdef ENOLINK
            case ENOLINK:
                return checkMask(Errno_ENOLINK, mappingMask);
        #endif
        #ifdef ENOMEDIUM
            case ENOMEDIUM:
                return checkMask(Errno_ENOMEDIUM, mappingMask);
        #endif
        #ifdef ENOMEM
            case ENOMEM:
                return checkMask(Errno_ENOMEM, mappingMask);
        #endif
        #ifdef ENOMSG
            case ENOMSG:
                return checkMask(Errno_ENOMSG, mappingMask);
        #endif
        #ifdef ENONET
            case ENONET:
                return checkMask(Errno_ENONET, mappingMask);
        #endif
        #ifdef ENOPKG
            case ENOPKG:
                return checkMask(Errno_ENOPKG, mappingMask);
        #endif
        #ifdef ENOPROTOOPT
            case ENOPROTOOPT:
                return checkMask(Errno_ENOPROTOOPT, mappingMask);
        #endif
        #ifdef ENOSPC
            case ENOSPC:
                return checkMask(Errno_ENOSPC, mappingMask);
        #endif
        #ifdef ENOSR
            case ENOSR:
                return checkMask(Errno_ENOSR, mappingMask);
        #endif
        #ifdef ENOSTR
            case ENOSTR:
                return checkMask(Errno_ENOSTR, mappingMask);
        #endif
        #ifdef ENOSYS
            case ENOSYS:
                return checkMask(Errno_ENOSYS, mappingMask);
        #endif
        #ifdef ENOTBLK
            case ENOTBLK:
                return checkMask(Errno_ENOTBLK, mappingMask);
        #endif
        #ifdef ENOTCAPABLE
            case ENOTCAPABLE:
                return checkMask(Errno_ENOTCAPABLE, mappingMask);
        #endif
        #ifdef ENOTCONN
            case ENOTCONN:
                return checkMask(Errno_ENOTCONN, mappingMask);
        #endif
        #ifdef ENOTDIR
            case ENOTDIR:
                return checkMask(Errno_ENOTDIR, mappingMask);
        #endif
        #ifdef ENOTEMPTY
            case ENOTEMPTY:
                return checkMask(Errno_ENOTEMPTY, mappingMask);
        #endif
        #ifdef ENOTNAM
            case ENOTNAM:
                return checkMask(Errno_ENOTNAM, mappingMask);
        #endif
        #ifdef ENOTRECOVERABLE
            case ENOTRECOVERABLE:
                return checkMask(Errno_ENOTRECOVERABLE, mappingMask);
        #endif
        #ifdef ENOTSOCK
            case ENOTSOCK:
                return checkMask(Errno_ENOTSOCK, mappingMask);
        #endif
        #ifdef ENOTSUP
            case ENOTSUP:
                return checkMask(Errno_ENOTSUP, mappingMask);
        #endif
        #ifdef ENOTTY
            case ENOTTY:
                return checkMask(Errno_ENOTTY, mappingMask);
        #endif
        #ifdef ENOTUNIQ
            case ENOTUNIQ:
                return checkMask(Errno_ENOTUNIQ, mappingMask);
        #endif
        #ifdef ENXIO
            case ENXIO:
                return checkMask(Errno_ENXIO, mappingMask);
        #endif
        #if defined(EOPNOTSUPP) && EOPNOTSUPP != ENOTSUP
            case EOPNOTSUPP:
                return checkMask(Errno_EOPNOTSUPP, mappingMask);
        #endif
        #ifdef EOVERFLOW
            case EOVERFLOW:
                return checkMask(Errno_EOVERFLOW, mappingMask);
        #endif
        #ifdef EOWNERDEAD
            case EOWNERDEAD:
                return checkMask(Errno_EOWNERDEAD, mappingMask);
        #endif
        #ifdef EPERM
            case EPERM:
                return checkMask(Errno_EPERM, mappingMask);
        #endif
        #ifdef EPFNOSUPPORT
            case EPFNOSUPPORT:
                return checkMask(Errno_EPFNOSUPPORT, mappingMask);
        #endif
        #ifdef EPIPE
            case EPIPE:
                return checkMask(Errno_EPIPE, mappingMask);
        #endif
        #ifdef EPROCLIM
            case EPROCLIM:
                return checkMask(Errno_EPROCLIM, mappingMask);
        #endif
        #ifdef EPROCUNAVAIL
            case EPROCUNAVAIL:
                return checkMask(Errno_EPROCUNAVAIL, mappingMask);
        #endif
        #ifdef EPROGMISMATCH
            case EPROGMISMATCH:
                return checkMask(Errno_EPROGMISMATCH, mappingMask);
        #endif
        #ifdef EPROGUNAVAIL
            case EPROGUNAVAIL:
                return checkMask(Errno_EPROGUNAVAIL, mappingMask);
        #endif
        #ifdef EPROTO
            case EPROTO:
                return checkMask(Errno_EPROTO, mappingMask);
        #endif
        #ifdef EPROTONOSUPPORT
            case EPROTONOSUPPORT:
                return checkMask(Errno_EPROTONOSUPPORT, mappingMask);
        #endif
        #ifdef EPROTOTYPE
            case EPROTOTYPE:
                return checkMask(Errno_EPROTOTYPE, mappingMask);
        #endif
        #ifdef ERANGE
            case ERANGE:
                return checkMask(Errno_ERANGE, mappingMask);
        #endif
        #ifdef EREMCHG
            case EREMCHG:
                return checkMask(Errno_EREMCHG, mappingMask);
        #endif
        #ifdef EREMOTE
            case EREMOTE:
                return checkMask(Errno_EREMOTE, mappingMask);
        #endif
        #ifdef EREMOTEIO
            case EREMOTEIO:
                return checkMask(Errno_EREMOTEIO, mappingMask);
        #endif
        #ifdef ERESTART
            case ERESTART:
                return checkMask(Errno_ERESTART, mappingMask);
        #endif
        #ifdef ERFKILL
            case ERFKILL:
                return checkMask(Errno_ERFKILL, mappingMask);
        #endif
        #ifdef EROFS
            case EROFS:
                return checkMask(Errno_EROFS, mappingMask);
        #endif
        #ifdef ERPCMISMATCH
            case ERPCMISMATCH:
                return checkMask(Errno_ERPCMISMATCH, mappingMask);
        #endif
        #ifdef ESHUTDOWN
            case ESHUTDOWN:
                return checkMask(Errno_ESHUTDOWN, mappingMask);
        #endif
        #ifdef ESOCKTNOSUPPORT
            case ESOCKTNOSUPPORT:
                return checkMask(Errno_ESOCKTNOSUPPORT, mappingMask);
        #endif
        #ifdef ESPIPE
            case ESPIPE:
                return checkMask(Errno_ESPIPE, mappingMask);
        #endif
        #ifdef ESRCH
            case ESRCH:
                return checkMask(Errno_ESRCH, mappingMask);
        #endif
        #ifdef ESRMNT
            case ESRMNT:
                return checkMask(Errno_ESRMNT, mappingMask);
        #endif
        #ifdef ESTALE
            case ESTALE:
                return checkMask(Errno_ESTALE, mappingMask);
        #endif
        #ifdef ESTRPIPE
            case ESTRPIPE:
                return checkMask(Errno_ESTRPIPE, mappingMask);
        #endif
        #ifdef ETIME
            case ETIME:
                return checkMask(Errno_ETIME, mappingMask);
        #endif
        #ifdef ETIMEDOUT
            case ETIMEDOUT:
                return checkMask(Errno_ETIMEDOUT, mappingMask);
        #endif
        #ifdef ETOOMANYREFS
            case ETOOMANYREFS:
                return checkMask(Errno_ETOOMANYREFS, mappingMask);
        #endif
        #ifdef ETXTBSY
            case ETXTBSY:
                return checkMask(Errno_ETXTBSY, mappingMask);
        #endif
        #ifdef EUCLEAN
            case EUCLEAN:
                return checkMask(Errno_EUCLEAN, mappingMask);
        #endif
        #ifdef EUNATCH
            case EUNATCH:
                return checkMask(Errno_EUNATCH, mappingMask);
        #endif
        #ifdef EUSERS
            case EUSERS:
                return checkMask(Errno_EUSERS, mappingMask);
        #endif
        #if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK:
                return checkMask(Errno_EWOULDBLOCK, mappingMask);
        #endif
        #ifdef EXDEV
            case EXDEV:
                return checkMask(Errno_EXDEV, mappingMask);
        #endif
        #ifdef EXFULL
            case EXFULL:
                return checkMask(Errno_EXFULL, mappingMask);
        #endif
        #ifdef WSA_INVALID_HANDLE
            case WSA_INVALID_HANDLE:
                return checkMask(Errno_WSA_INVALID_HANDLE, mappingMask);
        #endif
        #ifdef WSA_NOT_ENOUGH_MEMORY
            case WSA_NOT_ENOUGH_MEMORY:
                return checkMask(Errno_WSA_NOT_ENOUGH_MEMORY, mappingMask);
        #endif
        #ifdef WSA_INVALID_PARAMETER
            case WSA_INVALID_PARAMETER:
                return checkMask(Errno_WSA_INVALID_PARAMETER, mappingMask);
        #endif
        #ifdef WSA_OPERATION_ABORTED
            case WSA_OPERATION_ABORTED:
                return checkMask(Errno_WSA_OPERATION_ABORTED, mappingMask);
        #endif
        #ifdef WSA_IO_INCOMPLETE
            case WSA_IO_INCOMPLETE:
                return checkMask(Errno_WSA_IO_INCOMPLETE, mappingMask);
        #endif
        #ifdef WSA_IO_PENDING
            case WSA_IO_PENDING:
                return checkMask(Errno_WSA_IO_PENDING, mappingMask);
        #endif
        #ifdef WSAEINTR
            case WSAEINTR:
                return checkMask(Errno_WSAEINTR, mappingMask);
        #endif
        #ifdef WSAEBADF
            case WSAEBADF:
                return checkMask(Errno_WSAEBADF, mappingMask);
        #endif
        #ifdef WSAEACCES
            case WSAEACCES:
                return checkMask(Errno_WSAEACCES, mappingMask);
        #endif
        #ifdef WSAEFAULT
            case WSAEFAULT:
                return checkMask(Errno_WSAEFAULT, mappingMask);
        #endif
        #ifdef WSAEINVAL
            case WSAEINVAL:
                return checkMask(Errno_WSAEINVAL, mappingMask);
        #endif
        #ifdef WSAEMFILE
            case WSAEMFILE:
                return checkMask(Errno_WSAEMFILE, mappingMask);
        #endif
        #ifdef WSAEWOULDBLOCK
            case WSAEWOULDBLOCK:
                return checkMask(Errno_WSAEWOULDBLOCK, mappingMask);
        #endif
        #ifdef WSAEINPROGRESS
            case WSAEINPROGRESS:
                return checkMask(Errno_WSAEINPROGRESS, mappingMask);
        #endif
        #ifdef WSAEALREADY
            case WSAEALREADY:
                return checkMask(Errno_WSAEALREADY, mappingMask);
        #endif
        #ifdef WSAENOTSOCK
            case WSAENOTSOCK:
                return checkMask(Errno_WSAENOTSOCK, mappingMask);
        #endif
        #ifdef WSAEDESTADDRREQ
            case WSAEDESTADDRREQ:
                return checkMask(Errno_WSAEDESTADDRREQ, mappingMask);
        #endif
        #ifdef WSAEMSGSIZE
            case WSAEMSGSIZE:
                return checkMask(Errno_WSAEMSGSIZE, mappingMask);
        #endif
        #ifdef WSAEPROTOTYPE
            case WSAEPROTOTYPE:
                return checkMask(Errno_WSAEPROTOTYPE, mappingMask);
        #endif
        #ifdef WSAENOPROTOOPT
            case WSAENOPROTOOPT:
                return checkMask(Errno_WSAENOPROTOOPT, mappingMask);
        #endif
        #ifdef WSAEPROTONOSUPPORT
            case WSAEPROTONOSUPPORT:
                return checkMask(Errno_WSAEPROTONOSUPPORT, mappingMask);
        #endif
        #ifdef WSAESOCKTNOSUPPORT
            case WSAESOCKTNOSUPPORT:
                return checkMask(Errno_WSAESOCKTNOSUPPORT, mappingMask);
        #endif
        #ifdef WSAEOPNOTSUPP
            case WSAEOPNOTSUPP:
                return checkMask(Errno_WSAEOPNOTSUPP, mappingMask);
        #endif
        #ifdef WSAEPFNOSUPPORT
            case WSAEPFNOSUPPORT:
                return checkMask(Errno_WSAEPFNOSUPPORT, mappingMask);
        #endif
        #ifdef WSAEAFNOSUPPORT
            case WSAEAFNOSUPPORT:
                return checkMask(Errno_WSAEAFNOSUPPORT, mappingMask);
        #endif
        #ifdef WSAEADDRINUSE
            case WSAEADDRINUSE:
                return checkMask(Errno_WSAEADDRINUSE, mappingMask);
        #endif
        #ifdef WSAEADDRNOTAVAIL
            case WSAEADDRNOTAVAIL:
                return checkMask(Errno_WSAEADDRNOTAVAIL, mappingMask);
        #endif
        #ifdef WSAENETDOWN
            case WSAENETDOWN:
                return checkMask(Errno_WSAENETDOWN, mappingMask);
        #endif
        #ifdef WSAENETUNREACH
            case WSAENETUNREACH:
                return checkMask(Errno_WSAENETUNREACH, mappingMask);
        #endif
        #ifdef WSAENETRESET
            case WSAENETRESET:
                return checkMask(Errno_WSAENETRESET, mappingMask);
        #endif
        #ifdef WSAECONNABORTED
            case WSAECONNABORTED:
                return checkMask(Errno_WSAECONNABORTED, mappingMask);
        #endif
        #ifdef WSAECONNRESET
            case WSAECONNRESET:
                return checkMask(Errno_WSAECONNRESET, mappingMask);
        #endif
        #ifdef WSAENOBUFS
            case WSAENOBUFS:
                return checkMask(Errno_WSAENOBUFS, mappingMask);
        #endif
        #ifdef WSAEISCONN
            case WSAEISCONN:
                return checkMask(Errno_WSAEISCONN, mappingMask);
        #endif
        #ifdef WSAENOTCONN
            case WSAENOTCONN:
                return checkMask(Errno_WSAENOTCONN, mappingMask);
        #endif
        #ifdef WSAESHUTDOWN
            case WSAESHUTDOWN:
                return checkMask(Errno_WSAESHUTDOWN, mappingMask);
        #endif
        #ifdef WSAETOOMANYREFS
            case WSAETOOMANYREFS:
                return checkMask(Errno_WSAETOOMANYREFS, mappingMask);
        #endif
        #ifdef WSAETIMEDOUT
            case WSAETIMEDOUT:
                return checkMask(Errno_WSAETIMEDOUT, mappingMask);
        #endif
        #ifdef WSAECONNREFUSED
            case WSAECONNREFUSED:
                return checkMask(Errno_WSAECONNREFUSED, mappingMask);
        #endif
        #ifdef WSAELOOP
            case WSAELOOP:
                return checkMask(Errno_WSAELOOP, mappingMask);
        #endif
        #ifdef WSAENAMETOOLONG
            case WSAENAMETOOLONG:
                return checkMask(Errno_WSAENAMETOOLONG, mappingMask);
        #endif
        #ifdef WSAEHOSTDOWN
            case WSAEHOSTDOWN:
                return checkMask(Errno_WSAEHOSTDOWN, mappingMask);
        #endif
        #ifdef WSAEHOSTUNREACH
            case WSAEHOSTUNREACH:
                return checkMask(Errno_WSAEHOSTUNREACH, mappingMask);
        #endif
        #ifdef WSAENOTEMPTY
            case WSAENOTEMPTY:
                return checkMask(Errno_WSAENOTEMPTY, mappingMask);
        #endif
        #ifdef WSAEPROCLIM
            case WSAEPROCLIM:
                return checkMask(Errno_WSAEPROCLIM, mappingMask);
        #endif
        #ifdef WSAEUSERS
            case WSAEUSERS:
                return checkMask(Errno_WSAEUSERS, mappingMask);
        #endif
        #ifdef WSAEDQUOT
            case WSAEDQUOT:
                return checkMask(Errno_WSAEDQUOT, mappingMask);
        #endif
        #ifdef WSAESTALE
            case WSAESTALE:
                return checkMask(Errno_WSAESTALE, mappingMask);
        #endif
        #ifdef WSAEREMOTE
            case WSAEREMOTE:
                return checkMask(Errno_WSAEREMOTE, mappingMask);
        #endif
        #ifdef WSASYSNOTREADY
            case WSASYSNOTREADY:
                return checkMask(Errno_WSASYSNOTREADY, mappingMask);
        #endif
        #ifdef WSAVERNOTSUPPORTED
            case WSAVERNOTSUPPORTED:
                return checkMask(Errno_WSAVERNOTSUPPORTED, mappingMask);
        #endif
        #ifdef WSANOTINITIALISED
            case WSANOTINITIALISED:
                return checkMask(Errno_WSANOTINITIALISED, mappingMask);
        #endif
        #ifdef WSAEDISCON
            case WSAEDISCON:
                return checkMask(Errno_WSAEDISCON, mappingMask);
        #endif
        #ifdef WSAENOMORE
            case WSAENOMORE:
                return checkMask(Errno_WSAENOMORE, mappingMask);
        #endif
        #ifdef WSAECANCELLED
            case WSAECANCELLED:
                return checkMask(Errno_WSAECANCELLED, mappingMask);
        #endif
        #ifdef WSAEINVALIDPROCTABLE
            case WSAEINVALIDPROCTABLE:
                return checkMask(Errno_WSAEINVALIDPROCTABLE, mappingMask);
        #endif
        #ifdef WSAEINVALIDPROVIDER
            case WSAEINVALIDPROVIDER:
                return checkMask(Errno_WSAEINVALIDPROVIDER, mappingMask);
        #endif
        #ifdef WSAEPROVIDERFAILEDINIT
            case WSAEPROVIDERFAILEDINIT:
                return checkMask(Errno_WSAEPROVIDERFAILEDINIT, mappingMask);
        #endif
        #ifdef WSASYSCALLFAILURE
            case WSASYSCALLFAILURE:
                return checkMask(Errno_WSASYSCALLFAILURE, mappingMask);
        #endif
        #ifdef WSASERVICE_NOT_FOUND
            case WSASERVICE_NOT_FOUND:
                return checkMask(Errno_WSASERVICE_NOT_FOUND, mappingMask);
        #endif
        #ifdef WSATYPE_NOT_FOUND
            case WSATYPE_NOT_FOUND:
                return checkMask(Errno_WSATYPE_NOT_FOUND, mappingMask);
        #endif
        #ifdef WSA_E_NO_MORE
            case WSA_E_NO_MORE:
                return checkMask(Errno_WSA_E_NO_MORE, mappingMask);
        #endif
        #ifdef WSA_E_CANCELLED
            case WSA_E_CANCELLED:
                return checkMask(Errno_WSA_E_CANCELLED, mappingMask);
        #endif
        #ifdef WSAEREFUSED
            case WSAEREFUSED:
                return checkMask(Errno_WSAEREFUSED, mappingMask);
        #endif
        #ifdef WSAHOST_NOT_FOUND
            case WSAHOST_NOT_FOUND:
                return checkMask(Errno_WSAHOST_NOT_FOUND, mappingMask);
        #endif
        #ifdef WSATRY_AGAIN
            case WSATRY_AGAIN:
                return checkMask(Errno_WSATRY_AGAIN, mappingMask);
        #endif
        #ifdef WSANO_RECOVERY
            case WSANO_RECOVERY:
                return checkMask(Errno_WSANO_RECOVERY, mappingMask);
        #endif
        #ifdef WSANO_DATA
            case WSANO_DATA:
                return checkMask(Errno_WSANO_DATA, mappingMask);
        #endif
        #ifdef WSA_QOS_RECEIVERS
            case WSA_QOS_RECEIVERS:
                return checkMask(Errno_WSA_QOS_RECEIVERS, mappingMask);
        #endif
        #ifdef WSA_QOS_SENDERS
            case WSA_QOS_SENDERS:
                return checkMask(Errno_WSA_QOS_SENDERS, mappingMask);
        #endif
        #ifdef WSA_QOS_NO_SENDERS
            case WSA_QOS_NO_SENDERS:
                return checkMask(Errno_WSA_QOS_NO_SENDERS, mappingMask);
        #endif
        #ifdef WSA_QOS_NO_RECEIVERS
            case WSA_QOS_NO_RECEIVERS:
                return checkMask(Errno_WSA_QOS_NO_RECEIVERS, mappingMask);
        #endif
        #ifdef WSA_QOS_REQUEST_CONFIRMED
            case WSA_QOS_REQUEST_CONFIRMED:
                return checkMask(Errno_WSA_QOS_REQUEST_CONFIRMED, mappingMask);
        #endif
        #ifdef WSA_QOS_ADMISSION_FAILURE
            case WSA_QOS_ADMISSION_FAILURE:
                return checkMask(Errno_WSA_QOS_ADMISSION_FAILURE, mappingMask);
        #endif
        #ifdef WSA_QOS_POLICY_FAILURE
            case WSA_QOS_POLICY_FAILURE:
                return checkMask(Errno_WSA_QOS_POLICY_FAILURE, mappingMask);
        #endif
        #ifdef WSA_QOS_BAD_STYLE
            case WSA_QOS_BAD_STYLE:
                return checkMask(Errno_WSA_QOS_BAD_STYLE, mappingMask);
        #endif
        #ifdef WSA_QOS_BAD_OBJECT
            case WSA_QOS_BAD_OBJECT:
                return checkMask(Errno_WSA_QOS_BAD_OBJECT, mappingMask);
        #endif
        #ifdef WSA_QOS_TRAFFIC_CTRL_ERROR
            case WSA_QOS_TRAFFIC_CTRL_ERROR:
                return checkMask(Errno_WSA_QOS_TRAFFIC_CTRL_ERROR, mappingMask);
        #endif
        #ifdef WSA_QOS_GENERIC_ERROR
            case WSA_QOS_GENERIC_ERROR:
                return checkMask(Errno_WSA_QOS_GENERIC_ERROR, mappingMask);
        #endif
        #ifdef WSA_QOS_ESERVICETYPE
            case WSA_QOS_ESERVICETYPE:
                return checkMask(Errno_WSA_QOS_ESERVICETYPE, mappingMask);
        #endif
        #ifdef WSA_QOS_EFLOWSPEC
            case WSA_QOS_EFLOWSPEC:
                return checkMask(Errno_WSA_QOS_EFLOWSPEC, mappingMask);
        #endif
        #ifdef WSA_QOS_EPROVSPECBUF
            case WSA_QOS_EPROVSPECBUF:
                return checkMask(Errno_WSA_QOS_EPROVSPECBUF, mappingMask);
        #endif
        #ifdef WSA_QOS_EFILTERSTYLE
            case WSA_QOS_EFILTERSTYLE:
                return checkMask(Errno_WSA_QOS_EFILTERSTYLE, mappingMask);
        #endif
        #ifdef WSA_QOS_EFILTERTYPE
            case WSA_QOS_EFILTERTYPE:
                return checkMask(Errno_WSA_QOS_EFILTERTYPE, mappingMask);
        #endif
        #ifdef WSA_QOS_EFILTERCOUNT
            case WSA_QOS_EFILTERCOUNT:
                return checkMask(Errno_WSA_QOS_EFILTERCOUNT, mappingMask);
        #endif
        #ifdef WSA_QOS_EOBJLENGTH
            case WSA_QOS_EOBJLENGTH:
                return checkMask(Errno_WSA_QOS_EOBJLENGTH, mappingMask);
        #endif
        #ifdef WSA_QOS_EFLOWCOUNT
            case WSA_QOS_EFLOWCOUNT:
                return checkMask(Errno_WSA_QOS_EFLOWCOUNT, mappingMask);
        #endif
        #ifdef WSA_QOS_EUNKOWNPSOBJ
            case WSA_QOS_EUNKOWNPSOBJ:
                return checkMask(Errno_WSA_QOS_EUNKOWNPSOBJ, mappingMask);
        #endif
        #ifdef WSA_QOS_EPOLICYOBJ
            case WSA_QOS_EPOLICYOBJ:
                return checkMask(Errno_WSA_QOS_EPOLICYOBJ, mappingMask);
        #endif
        #ifdef WSA_QOS_EFLOWDESC
            case WSA_QOS_EFLOWDESC:
                return checkMask(Errno_WSA_QOS_EFLOWDESC, mappingMask);
        #endif
        #ifdef WSA_QOS_EPSFLOWSPEC
            case WSA_QOS_EPSFLOWSPEC:
                return checkMask(Errno_WSA_QOS_EPSFLOWSPEC, mappingMask);
        #endif
        #ifdef WSA_QOS_EPSFILTERSPEC
            case WSA_QOS_EPSFILTERSPEC:
                return checkMask(Errno_WSA_QOS_EPSFILTERSPEC, mappingMask);
        #endif
        #ifdef WSA_QOS_ESDMODEOBJ
            case WSA_QOS_ESDMODEOBJ:
                return checkMask(Errno_WSA_QOS_ESDMODEOBJ, mappingMask);
        #endif
        #ifdef WSA_QOS_ESHAPERATEOBJ
            case WSA_QOS_ESHAPERATEOBJ:
                return checkMask(Errno_WSA_QOS_ESHAPERATEOBJ, mappingMask);
        #endif
        #ifdef WSA_QOS_RESERVED_PETYPE
            case WSA_QOS_RESERVED_PETYPE:
                return checkMask(Errno_WSA_QOS_RESERVED_PETYPE, mappingMask);
        #endif
        default: return Errno_UNKNOWN;
    }
}


char* Errno_strerror(enum Errno err)
{
    switch (err) {
        case Errno_NO_ERROR: return "no error";
        case Errno_EINTR: return "interrupted system call";
        case Errno_ENOMEM: return "out of memory";
        case Errno_ENOENT: return "file does not exist";
        case Errno_ETXTBSY: return "text busy";
        case Errno_EIO: return "input/output error";
        case Errno_EEXIST: return "file already exists";
        case Errno_ETIMEDOUT: return "timed out";
        case Errno_EINPROGRESS: return "operation in progress";
        case Errno_EAGAIN: return "temporary failure";
        case Errno_EWOULDBLOCK: return "input/output would block";
        case Errno_EPIPE: return "broken pipe";
        case Errno_EPERM: return "permission denied";
        case Errno_EACCES: return "access denied";
        case Errno_EPROTO: return "protocol error";
        case Errno_EISDIR: return "is a directory";
        case Errno_ESRCH: return "no such process";
        case Errno_E2BIG: return "argument list too long";
        case Errno_ENOEXEC: return "exec format error";
        case Errno_EBADF: return "file descriptor not open";
        case Errno_ECHILD: return "no child processes";
        case Errno_EDEADLK: return "operation would cause deadlock";
        case Errno_EFAULT: return "bad address";
        case Errno_ENOTBLK: return "not a block device";
        case Errno_EBUSY: return "device busy";
        case Errno_EXDEV: return "cross-device link";
        case Errno_ENODEV: return "device does not support operation";
        case Errno_ENOTDIR: return "not a directory";
        case Errno_EINVAL: return "invalid argument";
        case Errno_ENFILE: return "system cannot open more files";
        case Errno_EMFILE: return "process cannot open more files";
        case Errno_ENOTTY: return "not a tty";
        case Errno_EFBIG: return "file too big";
        case Errno_ENOSPC: return "out of disk space";
        case Errno_ESPIPE: return "unseekable descriptor";
        case Errno_EROFS: return "read-only file system";
        case Errno_EMLINK: return "too many links";
        case Errno_EDOM: return "input out of range";
        case Errno_ERANGE: return "output out of range";
        case Errno_EALREADY: return "operation already in progress";
        case Errno_ENOTSOCK: return "not a socket";
        case Errno_EDESTADDRREQ: return "destination address required";
        case Errno_EMSGSIZE: return "message too long";
        case Errno_EPROTOTYPE: return "incorrect protocol type";
        case Errno_ENOPROTOOPT: return "protocol not available";
        case Errno_EPROTONOSUPPORT: return "protocol not supported";
        case Errno_ESOCKTNOSUPPORT: return "socket type not supported";
        case Errno_EOPNOTSUPP: return "operation not supported";
        case Errno_EPFNOSUPPORT: return "protocol family not supported";
        case Errno_EAFNOSUPPORT: return "address family not supported";
        case Errno_EADDRINUSE: return "address already used";
        case Errno_EADDRNOTAVAIL: return "address not available";
        case Errno_ENETDOWN: return "network down";
        case Errno_ENETUNREACH: return "network unreachable";
        case Errno_ENETRESET: return "network reset";
        case Errno_ECONNABORTED: return "connection aborted";
        case Errno_ECONNRESET: return "connection reset";
        case Errno_ENOBUFS: return "out of buffer space";
        case Errno_EISCONN: return "already connected";
        case Errno_ENOTCONN: return "not connected";
        case Errno_ESHUTDOWN: return "socket shut down";
        case Errno_ETOOMANYREFS: return "too many references";
        case Errno_ECONNREFUSED: return "connection refused";
        case Errno_ELOOP: return "symbolic link loop";
        case Errno_ENAMETOOLONG: return "file name too long";
        case Errno_EHOSTDOWN: return "host down";
        case Errno_EHOSTUNREACH: return "host unreachable";
        case Errno_ENOTEMPTY: return "directory not empty";
        case Errno_EPROCLIM: return "too many processes";
        case Errno_EUSERS: return "too many users";
        case Errno_EDQUOT: return "disk quota exceeded";
        case Errno_ESTALE: return "stale NFS file handle";
        case Errno_EBADRPC: return "RPC structure is bad";
        case Errno_ERPCMISMATCH: return "RPC version mismatch";
        case Errno_EPROGUNAVAIL: return "RPC program unavailable";
        case Errno_EPROGMISMATCH: return "program version mismatch";
        case Errno_EPROCUNAVAIL: return "bad procedure for program";
        case Errno_ENOLCK: return "no locks available";
        case Errno_ENOSYS: return "system call not available";
        case Errno_EFTYPE: return "bad file type";
        case Errno_EAUTH: return "authentication error";
        case Errno_ENEEDAUTH: return "not authenticated";
        case Errno_ENOSTR: return "not a stream device";
        case Errno_ETIME: return "timer expired";
        case Errno_ENOSR: return "out of stream resources";
        case Errno_ENOMSG: return "no message of desired type";
        case Errno_EBADMSG: return "bad message type";
        case Errno_EIDRM: return "identifier removed";
        case Errno_ENONET: return "machine not on network";
        case Errno_EREMOTE: return "object not local";
        case Errno_ENOLINK: return "link severed";
        case Errno_EADV: return "advertise error";
        case Errno_ESRMNT: return "srmount error";
        case Errno_ECOMM: return "communication error";
        case Errno_EMULTIHOP: return "multihop attempted";
        case Errno_EREMCHG: return "remote address changed";

        // TODO: needs documentation
        case Errno_EBADE: return "Errno_EBADE";
        case Errno_EBADFD: return "Errno_EBADFD";
        case Errno_EBADRQC: return "Errno_EBADRQC";
        case Errno_EBADSLT: return "Errno_EBADSLT";
        case Errno_EBFONT: return "Errno_EBFONT";
        case Errno_ECANCELED: return "Errno_ECANCELED";
        case Errno_ECHRNG: return "Errno_ECHRNG";
        case Errno_EDEADLOCK: return "Errno_EDEADLOCK";
        case Errno_EDOTDOT: return "Errno_EDOTDOT";
        case Errno_EILSEQ: return "Errno_EILSEQ";
        case Errno_EISNAM: return "Errno_EISNAM";
        case Errno_EKEYEXPIRED: return "Errno_EKEYEXPIRED";
        case Errno_EKEYREJECTED: return "Errno_EKEYREJECTED";
        case Errno_EKEYREVOKED: return "Errno_EKEYREVOKED";
        case Errno_EL2HLT: return "Errno_EL2HLT";
        case Errno_EL2NSYNC: return "Errno_EL2NSYNC";
        case Errno_EL3HLT: return "Errno_EL3HLT";
        case Errno_EL3RST: return "Errno_EL3RST";
        case Errno_ELIBACC: return "Errno_ELIBACC";
        case Errno_ELIBBAD: return "Errno_ELIBBAD";
        case Errno_ELIBEXEC: return "Errno_ELIBEXEC";
        case Errno_ELIBMAX: return "Errno_ELIBMAX";
        case Errno_ELIBSCN: return "Errno_ELIBSCN";
        case Errno_ELNRNG: return "Errno_ELNRNG";
        case Errno_EMEDIUMTYPE: return "Errno_EMEDIUMTYPE";
        case Errno_ENAVAIL: return "Errno_ENAVAIL";
        case Errno_ENOANO: return "Errno_ENOANO";
        case Errno_ENOATTR: return "Errno_ENOATTR";
        case Errno_ENOCSI: return "Errno_ENOCSI";
        case Errno_ENODATA: return "Errno_ENODATA";
        case Errno_ENOKEY: return "Errno_ENOKEY";
        case Errno_ENOMEDIUM: return "Errno_ENOMEDIUM";
        case Errno_ENOPKG: return "Errno_ENOPKG";
        case Errno_ENOTCAPABLE: return "Errno_ENOTCAPABLE";
        case Errno_ENOTNAM: return "Errno_ENOTNAM";
        case Errno_ENOTRECOVERABLE: return "Errno_ENOTRECOVERABLE";
        case Errno_ENOTSUP: return "Errno_ENOTSUP";
        case Errno_ENOTUNIQ: return "Errno_ENOTUNIQ";
        case Errno_ENXIO: return "Errno_ENXIO";
        case Errno_EOVERFLOW: return "Errno_EOVERFLOW";
        case Errno_EOWNERDEAD: return "Errno_EOWNERDEAD";
        case Errno_EREMOTEIO: return "Errno_EREMOTEIO";
        case Errno_ERESTART: return "Errno_ERESTART";
        case Errno_ERFKILL: return "Errno_ERFKILL";
        case Errno_ESTRPIPE: return "Errno_ESTRPIPE";
        case Errno_EUCLEAN: return "Errno_EUCLEAN";
        case Errno_EUNATCH: return "Errno_EUNATCH";
        case Errno_EXFULL: return "Errno_EXFULL";

        #ifdef Windows
            case Errno_WSA_INVALID_HANDLE: return "Errno_WSA_INVALID_HANDLE";
            case Errno_WSA_NOT_ENOUGH_MEMORY: return "Errno_WSA_NOT_ENOUGH_MEMORY";
            case Errno_WSA_INVALID_PARAMETER: return "Errno_WSA_INVALID_PARAMETER";
            case Errno_WSA_OPERATION_ABORTED: return "Errno_WSA_OPERATION_ABORTED";
            case Errno_WSA_IO_INCOMPLETE: return "Errno_WSA_IO_INCOMPLETE";
            case Errno_WSA_IO_PENDING: return "Errno_WSA_IO_PENDING";
            case Errno_WSAEINTR: return "Errno_WSAEINTR";
            case Errno_WSAEBADF: return "Errno_WSAEBADF";
            case Errno_WSAEACCES: return "Errno_WSAEACCES";
            case Errno_WSAEFAULT: return "Errno_WSAEFAULT";
            case Errno_WSAEINVAL: return "Errno_WSAEINVAL";
            case Errno_WSAEMFILE: return "Errno_WSAEMFILE";
            case Errno_WSAEWOULDBLOCK: return "Errno_WSAEWOULDBLOCK";
            case Errno_WSAEINPROGRESS: return "Errno_WSAEINPROGRESS";
            case Errno_WSAEALREADY: return "Errno_WSAEALREADY";
            case Errno_WSAENOTSOCK: return "Errno_WSAENOTSOCK";
            case Errno_WSAEDESTADDRREQ: return "Errno_WSAEDESTADDRREQ";
            case Errno_WSAEMSGSIZE: return "Errno_WSAEMSGSIZE";
            case Errno_WSAEPROTOTYPE: return "Errno_WSAEPROTOTYPE";
            case Errno_WSAENOPROTOOPT: return "Errno_WSAENOPROTOOPT";
            case Errno_WSAEPROTONOSUPPORT: return "Errno_WSAEPROTONOSUPPORT";
            case Errno_WSAESOCKTNOSUPPORT: return "Errno_WSAESOCKTNOSUPPORT";
            case Errno_WSAEOPNOTSUPP: return "Errno_WSAEOPNOTSUPP";
            case Errno_WSAEPFNOSUPPORT: return "Errno_WSAEPFNOSUPPORT";
            case Errno_WSAEAFNOSUPPORT: return "Errno_WSAEAFNOSUPPORT";
            case Errno_WSAEADDRINUSE: return "Errno_WSAEADDRINUSE";
            case Errno_WSAEADDRNOTAVAIL: return "Errno_WSAEADDRNOTAVAIL";
            case Errno_WSAENETDOWN: return "Errno_WSAENETDOWN";
            case Errno_WSAENETUNREACH: return "Errno_WSAENETUNREACH";
            case Errno_WSAENETRESET: return "Errno_WSAENETRESET";
            case Errno_WSAECONNABORTED: return "Errno_WSAECONNABORTED";
            case Errno_WSAECONNRESET: return "Errno_WSAECONNRESET";
            case Errno_WSAENOBUFS: return "Errno_WSAENOBUFS";
            case Errno_WSAEISCONN: return "Errno_WSAEISCONN";
            case Errno_WSAENOTCONN: return "Errno_WSAENOTCONN";
            case Errno_WSAESHUTDOWN: return "Errno_WSAESHUTDOWN";
            case Errno_WSAETOOMANYREFS: return "Errno_WSAETOOMANYREFS";
            case Errno_WSAETIMEDOUT: return "Errno_WSAETIMEDOUT";
            case Errno_WSAECONNREFUSED: return "Errno_WSAECONNREFUSED";
            case Errno_WSAELOOP: return "Errno_WSAELOOP";
            case Errno_WSAENAMETOOLONG: return "Errno_WSAENAMETOOLONG";
            case Errno_WSAEHOSTDOWN: return "Errno_WSAEHOSTDOWN";
            case Errno_WSAEHOSTUNREACH: return "Errno_WSAEHOSTUNREACH";
            case Errno_WSAENOTEMPTY: return "Errno_WSAENOTEMPTY";
            case Errno_WSAEPROCLIM: return "Errno_WSAEPROCLIM";
            case Errno_WSAEUSERS: return "Errno_WSAEUSERS";
            case Errno_WSAEDQUOT: return "Errno_WSAEDQUOT";
            case Errno_WSAESTALE: return "Errno_WSAESTALE";
            case Errno_WSAEREMOTE: return "Errno_WSAEREMOTE";
            case Errno_WSASYSNOTREADY: return "Errno_WSASYSNOTREADY";
            case Errno_WSAVERNOTSUPPORTED: return "Errno_WSAVERNOTSUPPORTED";
            case Errno_WSANOTINITIALISED: return "Errno_WSANOTINITIALISED";
            case Errno_WSAEDISCON: return "Errno_WSAEDISCON";
            case Errno_WSAENOMORE: return "Errno_WSAENOMORE";
            case Errno_WSAECANCELLED: return "Errno_WSAECANCELLED";
            case Errno_WSAEINVALIDPROCTABLE: return "Errno_WSAEINVALIDPROCTABLE";
            case Errno_WSAEINVALIDPROVIDER: return "Errno_WSAEINVALIDPROVIDER";
            case Errno_WSAEPROVIDERFAILEDINIT: return "Errno_WSAEPROVIDERFAILEDINIT";
            case Errno_WSASYSCALLFAILURE: return "Errno_WSASYSCALLFAILURE";
            case Errno_WSASERVICE_NOT_FOUND: return "Errno_WSASERVICE_NOT_FOUND";
            case Errno_WSATYPE_NOT_FOUND: return "Errno_WSATYPE_NOT_FOUND";
            case Errno_WSA_E_NO_MORE: return "Errno_WSA_E_NO_MORE";
            case Errno_WSA_E_CANCELLED: return "Errno_WSA_E_CANCELLED";
            case Errno_WSAEREFUSED: return "Errno_WSAEREFUSED";
            case Errno_WSAHOST_NOT_FOUND: return "Errno_WSAHOST_NOT_FOUND";
            case Errno_WSATRY_AGAIN: return "Errno_WSATRY_AGAIN";
            case Errno_WSANO_RECOVERY: return "Errno_WSANO_RECOVERY";
            case Errno_WSANO_DATA: return "Errno_WSANO_DATA";
            case Errno_WSA_QOS_RECEIVERS: return "Errno_WSA_QOS_RECEIVERS";
            case Errno_WSA_QOS_SENDERS: return "Errno_WSA_QOS_SENDERS";
            case Errno_WSA_QOS_NO_SENDERS: return "Errno_WSA_QOS_NO_SENDERS";
            case Errno_WSA_QOS_NO_RECEIVERS: return "Errno_WSA_QOS_NO_RECEIVERS";
            case Errno_WSA_QOS_REQUEST_CONFIRMED: return "Errno_WSA_QOS_REQUEST_CONFIRMED";
            case Errno_WSA_QOS_ADMISSION_FAILURE: return "Errno_WSA_QOS_ADMISSION_FAILURE";
            case Errno_WSA_QOS_POLICY_FAILURE: return "Errno_WSA_QOS_POLICY_FAILURE";
            case Errno_WSA_QOS_BAD_STYLE: return "Errno_WSA_QOS_BAD_STYLE";
            case Errno_WSA_QOS_BAD_OBJECT: return "Errno_WSA_QOS_BAD_OBJECT";
            case Errno_WSA_QOS_TRAFFIC_CTRL_ERROR: return "Errno_WSA_QOS_TRAFFIC_CTRL_ERROR";
            case Errno_WSA_QOS_GENERIC_ERROR: return "Errno_WSA_QOS_GENERIC_ERROR";
            case Errno_WSA_QOS_ESERVICETYPE: return "Errno_WSA_QOS_ESERVICETYPE";
            case Errno_WSA_QOS_EFLOWSPEC: return "Errno_WSA_QOS_EFLOWSPEC";
            case Errno_WSA_QOS_EPROVSPECBUF: return "Errno_WSA_QOS_EPROVSPECBUF";
            case Errno_WSA_QOS_EFILTERSTYLE: return "Errno_WSA_QOS_EFILTERSTYLE";
            case Errno_WSA_QOS_EFILTERTYPE: return "Errno_WSA_QOS_EFILTERTYPE";
            case Errno_WSA_QOS_EFILTERCOUNT: return "Errno_WSA_QOS_EFILTERCOUNT";
            case Errno_WSA_QOS_EOBJLENGTH: return "Errno_WSA_QOS_EOBJLENGTH";
            case Errno_WSA_QOS_EFLOWCOUNT: return "Errno_WSA_QOS_EFLOWCOUNT";
            case Errno_WSA_QOS_EUNKOWNPSOBJ: return "Errno_WSA_QOS_EUNKOWNPSOBJ";
            case Errno_WSA_QOS_EPOLICYOBJ: return "Errno_WSA_QOS_EPOLICYOBJ";
            case Errno_WSA_QOS_EFLOWDESC: return "Errno_WSA_QOS_EFLOWDESC";
            case Errno_WSA_QOS_EPSFLOWSPEC: return "Errno_WSA_QOS_EPSFLOWSPEC";
            case Errno_WSA_QOS_EPSFILTERSPEC: return "Errno_WSA_QOS_EPSFILTERSPEC";
            case Errno_WSA_QOS_ESDMODEOBJ: return "Errno_WSA_QOS_ESDMODEOBJ";
            case Errno_WSA_QOS_ESHAPERATEOBJ: return "Errno_WSA_QOS_ESHAPERATEOBJ";
            case Errno_WSA_QOS_RESERVED_PETYPE: return "Errno_WSA_QOS_RESERVED_PETYPE";
        #endif
        default: return "unknown error";
    }
}
