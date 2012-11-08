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

#include <stdint.h>


enum Errno
{
    Errno_NO_ERROR = 0,
    Errno_UNKNOWN,
    Errno_E2BIG,
    Errno_EACCES,
    Errno_EADDRINUSE,
    Errno_EADDRNOTAVAIL,
    Errno_EADV,
    Errno_EAFNOSUPPORT,
    Errno_EAGAIN,
    Errno_EALREADY,
    Errno_EAUTH,
    Errno_EBADE,
    Errno_EBADF,
    Errno_EBADFD,
    Errno_EBADMSG,
    Errno_EBADR,
    Errno_EBADRPC,
    Errno_EBADRQC,
    Errno_EBADSLT,
    Errno_EBFONT,
    Errno_EBUSY,
    Errno_ECANCELED,
    Errno_ECHILD,
    Errno_ECHRNG,
    Errno_ECOMM,
    Errno_ECONNABORTED,
    Errno_ECONNREFUSED,
    Errno_ECONNRESET,
    Errno_EDEADLK,
    Errno_EDEADLOCK,
    Errno_EDESTADDRREQ,
    Errno_EDOM,
    Errno_EDOTDOT,
    Errno_EDQUOT,
    Errno_EEXIST,
    Errno_EFAULT,
    Errno_EFBIG,
    Errno_EFTYPE,
    Errno_EHOSTDOWN,
    Errno_EHOSTUNREACH,
    Errno_EIDRM,
    Errno_EILSEQ,
    Errno_EINPROGRESS,
    Errno_EINTR,
    Errno_EINVAL,
    Errno_EIO,
    Errno_EISCONN,
    Errno_EISDIR,
    Errno_EISNAM,
    Errno_EKEYEXPIRED,
    Errno_EKEYREJECTED,
    Errno_EKEYREVOKED,
    Errno_EL2HLT,
    Errno_EL2NSYNC,
    Errno_EL3HLT,
    Errno_EL3RST,
    Errno_ELIBACC,
    Errno_ELIBBAD,
    Errno_ELIBEXEC,
    Errno_ELIBMAX,
    Errno_ELIBSCN,
    Errno_ELNRNG,
    Errno_ELOOP,
    Errno_EMEDIUMTYPE,
    Errno_EMFILE,
    Errno_EMLINK,
    Errno_EMSGSIZE,
    Errno_EMULTIHOP,
    Errno_ENAMETOOLONG,
    Errno_ENAVAIL,
    Errno_ENEEDAUTH,
    Errno_ENETDOWN,
    Errno_ENETRESET,
    Errno_ENETUNREACH,
    Errno_ENFILE,
    Errno_ENOANO,
    Errno_ENOATTR,
    Errno_ENOBUFS,
    Errno_ENOCSI,
    Errno_ENODATA,
    Errno_ENODEV,
    Errno_ENOENT,
    Errno_ENOEXEC,
    Errno_ENOKEY,
    Errno_ENOLCK,
    Errno_ENOLINK,
    Errno_ENOMEDIUM,
    Errno_ENOMEM,
    Errno_ENOMSG,
    Errno_ENONET,
    Errno_ENOPKG,
    Errno_ENOPROTOOPT,
    Errno_ENOSPC,
    Errno_ENOSR,
    Errno_ENOSTR,
    Errno_ENOSYS,
    Errno_ENOTBLK,
    Errno_ENOTCAPABLE,
    Errno_ENOTCONN,
    Errno_ENOTDIR,
    Errno_ENOTEMPTY,
    Errno_ENOTNAM,
    Errno_ENOTRECOVERABLE,
    Errno_ENOTSOCK,
    Errno_ENOTSUP,
    Errno_ENOTTY,
    Errno_ENOTUNIQ,
    Errno_ENXIO,
    Errno_EOPNOTSUPP,
    Errno_EOVERFLOW,
    Errno_EOWNERDEAD,
    Errno_EPERM,
    Errno_EPFNOSUPPORT,
    Errno_EPIPE,
    Errno_EPROCLIM,
    Errno_EPROCUNAVAIL,
    Errno_EPROGMISMATCH,
    Errno_EPROGUNAVAIL,
    Errno_EPROTO,
    Errno_EPROTONOSUPPORT,
    Errno_EPROTOTYPE,
    Errno_ERANGE,
    Errno_EREMCHG,
    Errno_EREMOTE,
    Errno_EREMOTEIO,
    Errno_ERESTART,
    Errno_ERFKILL,
    Errno_EROFS,
    Errno_ERPCMISMATCH,
    Errno_ESHUTDOWN,
    Errno_ESOCKTNOSUPPORT,
    Errno_ESPIPE,
    Errno_ESRCH,
    Errno_ESRMNT,
    Errno_ESTALE,
    Errno_ESTRPIPE,
    Errno_ETIME,
    Errno_ETIMEDOUT,
    Errno_ETOOMANYREFS,
    Errno_ETXTBSY,
    Errno_EUCLEAN,
    Errno_EUNATCH,
    Errno_EUSERS,
    Errno_EWOULDBLOCK,
    Errno_EXDEV,
    Errno_EXFULL,

    // winsock
    // These exist even if the platform is non-windows
    // to keep numbers from colliding.
    Errno_WSA_INVALID_HANDLE,
    Errno_WSA_NOT_ENOUGH_MEMORY,
    Errno_WSA_INVALID_PARAMETER,
    Errno_WSA_OPERATION_ABORTED,
    Errno_WSA_IO_INCOMPLETE,
    Errno_WSA_IO_PENDING,
    Errno_WSAEINTR,
    Errno_WSAEBADF,
    Errno_WSAEACCES,
    Errno_WSAEFAULT,
    Errno_WSAEINVAL,
    Errno_WSAEMFILE,
    Errno_WSAEWOULDBLOCK,
    Errno_WSAEINPROGRESS,
    Errno_WSAEALREADY,
    Errno_WSAENOTSOCK,
    Errno_WSAEDESTADDRREQ,
    Errno_WSAEMSGSIZE,
    Errno_WSAEPROTOTYPE,
    Errno_WSAENOPROTOOPT,
    Errno_WSAEPROTONOSUPPORT,
    Errno_WSAESOCKTNOSUPPORT,
    Errno_WSAEOPNOTSUPP,
    Errno_WSAEPFNOSUPPORT,
    Errno_WSAEAFNOSUPPORT,
    Errno_WSAEADDRINUSE,
    Errno_WSAEADDRNOTAVAIL,
    Errno_WSAENETDOWN,
    Errno_WSAENETUNREACH,
    Errno_WSAENETRESET,
    Errno_WSAECONNABORTED,
    Errno_WSAECONNRESET,
    Errno_WSAENOBUFS,
    Errno_WSAEISCONN,
    Errno_WSAENOTCONN,
    Errno_WSAESHUTDOWN,
    Errno_WSAETOOMANYREFS,
    Errno_WSAETIMEDOUT,
    Errno_WSAECONNREFUSED,
    Errno_WSAELOOP,
    Errno_WSAENAMETOOLONG,
    Errno_WSAEHOSTDOWN,
    Errno_WSAEHOSTUNREACH,
    Errno_WSAENOTEMPTY,
    Errno_WSAEPROCLIM,
    Errno_WSAEUSERS,
    Errno_WSAEDQUOT,
    Errno_WSAESTALE,
    Errno_WSAEREMOTE,
    Errno_WSASYSNOTREADY,
    Errno_WSAVERNOTSUPPORTED,
    Errno_WSANOTINITIALISED,
    Errno_WSAEDISCON,
    Errno_WSAENOMORE,
    Errno_WSAECANCELLED,
    Errno_WSAEINVALIDPROCTABLE,
    Errno_WSAEINVALIDPROVIDER,
    Errno_WSAEPROVIDERFAILEDINIT,
    Errno_WSASYSCALLFAILURE,
    Errno_WSASERVICE_NOT_FOUND,
    Errno_WSATYPE_NOT_FOUND,
    Errno_WSA_E_NO_MORE,
    Errno_WSA_E_CANCELLED,
    Errno_WSAEREFUSED,
    Errno_WSAHOST_NOT_FOUND,
    Errno_WSATRY_AGAIN,
    Errno_WSANO_RECOVERY,
    Errno_WSANO_DATA,
    Errno_WSA_QOS_RECEIVERS,
    Errno_WSA_QOS_SENDERS,
    Errno_WSA_QOS_NO_SENDERS,
    Errno_WSA_QOS_NO_RECEIVERS,
    Errno_WSA_QOS_REQUEST_CONFIRMED,
    Errno_WSA_QOS_ADMISSION_FAILURE,
    Errno_WSA_QOS_POLICY_FAILURE,
    Errno_WSA_QOS_BAD_STYLE,
    Errno_WSA_QOS_BAD_OBJECT,
    Errno_WSA_QOS_TRAFFIC_CTRL_ERROR,
    Errno_WSA_QOS_GENERIC_ERROR,
    Errno_WSA_QOS_ESERVICETYPE,
    Errno_WSA_QOS_EFLOWSPEC,
    Errno_WSA_QOS_EPROVSPECBUF,
    Errno_WSA_QOS_EFILTERSTYLE,
    Errno_WSA_QOS_EFILTERTYPE,
    Errno_WSA_QOS_EFILTERCOUNT,
    Errno_WSA_QOS_EOBJLENGTH,
    Errno_WSA_QOS_EFLOWCOUNT,
    Errno_WSA_QOS_EUNKOWNPSOBJ,
    Errno_WSA_QOS_EPOLICYOBJ,
    Errno_WSA_QOS_EFLOWDESC,
    Errno_WSA_QOS_EPSFLOWSPEC,
    Errno_WSA_QOS_EPSFILTERSPEC,
    Errno_WSA_QOS_ESDMODEOBJ,
    Errno_WSA_QOS_ESHAPERATEOBJ,
    Errno_WSA_QOS_RESERVED_PETYPE
};

enum Errno_Mappings
{
    Errno_Mappings_ENOTSUP,
    Errno_Mappings_EWOULDBLOCK,
    Errno_Mappings_EDEADLK,

    // Winsock.
    Errno_Mappings_WSAEWOULDBLOCK,
    Errno_Mappings_WSAEINPROGRESS,
    Errno_Mappings_WSAEALREADY,
    Errno_Mappings_WSAENOTSOCK,
    Errno_Mappings_WSAEDESTADDRREQ,
    Errno_Mappings_WSAEMSGSIZE,
    Errno_Mappings_WSAEPROTOTYPE,
    Errno_Mappings_WSAENOPROTOOPT,
    Errno_Mappings_WSAEPROTONOSUPPORT,
    Errno_Mappings_WSAESOCKTNOSUPPORT,
    Errno_Mappings_WSAEOPNOTSUPP,
    Errno_Mappings_WSAEPFNOSUPPORT,
    Errno_Mappings_WSAEAFNOSUPPORT,
    Errno_Mappings_WSAEADDRINUSE,
    Errno_Mappings_WSAEADDRNOTAVAIL,
    Errno_Mappings_WSAENETDOWN,
    Errno_Mappings_WSAENETUNREACH,
    Errno_Mappings_WSAENETRESET,
    Errno_Mappings_WSAECONNABORTED,
    Errno_Mappings_WSAECONNRESET,
    Errno_Mappings_WSAENOBUFS,
    Errno_Mappings_WSAEISCONN,
    Errno_Mappings_WSAENOTCONN,
    Errno_Mappings_WSAESHUTDOWN,
    Errno_Mappings_WSAETOOMANYREFS,
    Errno_Mappings_WSAETIMEDOUT,
    Errno_Mappings_WSAECONNREFUSED,
    Errno_Mappings_WSAELOOP,
    Errno_Mappings_WSAENAMETOOLONG,
    Errno_Mappings_WSAEHOSTDOWN,
    Errno_Mappings_WSAEHOSTUNREACH,
    Errno_Mappings_WSAENOTEMPTY,
    Errno_Mappings_WSAEPROCLIM,
    Errno_Mappings_WSAEUSERS,
    Errno_Mappings_WSAEDQUOT,
    Errno_Mappings_WSAESTALE,
    Errno_Mappings_WSAEREMOTE
};

// Errno.c defines NO_MAPPINGS so that it can refer to the enum constants directly.
#ifndef Errno_NO_MAPPINGS

    #ifndef Errno_DISTINCT_ENOTSUP
        #define Errno_ENOTSUP Errno_EOPNOTSUPP
    #endif
    #ifndef Errno_DISTINCT_EWULDBLOCK
        #define Errno_EWOULDBLOCK Errno_EAGAIN
    #endif
    #ifndef Errno_DISTINCT_EDEADLK
        #define Errno_EDEADLK Errno_EDEADLOCK
    #endif


    // Winsock
    #ifdef W32
        #ifndef Errno_DISTINCT_WSAEWOULDBLOCK
            #define Errno_WSAEWOULDBLOCK Errno_EWOULDBLOCK
        #endif
        #ifndef Errno_DISTINCT_WSAEINPROGRESS
            #define Errno_WSAEINPROGRESS Errno_EINPROGRESS
        #endif
        #ifndef Errno_DISTINCT_WSAEALREADY
            #define Errno_WSAEALREADY Errno_EALREADY
        #endif
        #ifndef Errno_DISTINCT_WSAENOTSOCK
            #define Errno_WSAENOTSOCK Errno_ENOTSOCK
        #endif
        #ifndef Errno_DISTINCT_WSAEDESTADDRREQ
            #define Errno_WSAEDESTADDRREQ Errno_EDESTADDRREQ
        #endif
        #ifndef Errno_DISTINCT_WSAEMSGSIZE
            #define Errno_WSAEMSGSIZE Errno_EMSGSIZE
        #endif
        #ifndef Errno_DISTINCT_WSAEPROTOTYPE
            #define Errno_WSAEPROTOTYPE Errno_EPROTOTYPE
        #endif
        #ifndef Errno_DISTINCT_WSAENOPROTOOPT
            #define Errno_WSAENOPROTOOPT Errno_ENOPROTOOPT
        #endif
        #ifndef Errno_DISTINCT_WSAEPROTONOSUPPORT
            #define Errno_WSAEPROTONOSUPPORT Errno_EPROTONOSUPPORT
        #endif
        #ifndef Errno_DISTINCT_WSAESOCKTNOSUPPORT
            #define Errno_WSAESOCKTNOSUPPORT Errno_ESOCKTNOSUPPORT
        #endif
        #ifndef Errno_DISTINCT_WSAEOPNOTSUPP
            #define Errno_WSAEOPNOTSUPP Errno_EOPNOTSUPP
        #endif
        #ifndef Errno_DISTINCT_WSAEPFNOSUPPORT
            #define Errno_WSAEPFNOSUPPORT Errno_EPFNOSUPPORT
        #endif
        #ifndef Errno_DISTINCT_WSAEAFNOSUPPORT
            #define Errno_WSAEAFNOSUPPORT Errno_EAFNOSUPPORT
        #endif
        #ifndef Errno_DISTINCT_WSAEADDRINUSE
            #define Errno_WSAEADDRINUSE Errno_EADDRINUSE
        #endif
        #ifndef Errno_DISTINCT_WSAEADDRNOTAVAIL
            #define Errno_WSAEADDRNOTAVAIL Errno_EADDRNOTAVAIL
        #endif
        #ifndef Errno_DISTINCT_WSAENETDOWN
            #define Errno_WSAENETDOWN Errno_ENETDOWN
        #endif
        #ifndef Errno_DISTINCT_WSAENETUNREACH
            #define Errno_WSAENETUNREACH Errno_ENETUNREACH
        #endif
        #ifndef Errno_DISTINCT_WSAENETRESET
            #define Errno_WSAENETRESET Errno_ENETRESET
        #endif
        #ifndef Errno_DISTINCT_WSAECONNABORTED
            #define Errno_WSAECONNABORTED Errno_ECONNABORTED
        #endif
        #ifndef Errno_DISTINCT_WSAECONNRESET
            #define Errno_WSAECONNRESET Errno_ECONNRESET
        #endif
        #ifndef Errno_DISTINCT_WSAENOBUFS
            #define Errno_WSAENOBUFS Errno_ENOBUFS
        #endif
        #ifndef Errno_DISTINCT_WSAEISCONN
            #define Errno_WSAEISCONN Errno_EISCONN
        #endif
        #ifndef Errno_DISTINCT_WSAENOTCONN
            #define Errno_WSAENOTCONN Errno_ENOTCONN
        #endif
        #ifndef Errno_DISTINCT_WSAESHUTDOWN
            #define Errno_WSAESHUTDOWN Errno_ESHUTDOWN
        #endif
        #ifndef Errno_DISTINCT_WSAETOOMANYREFS
            #define Errno_WSAETOOMANYREFS Errno_ETOOMANYREFS
        #endif
        #ifndef Errno_DISTINCT_WSAETIMEDOUT
            #define Errno_WSAETIMEDOUT Errno_ETIMEDOUT
        #endif
        #ifndef Errno_DISTINCT_WSAECONNREFUSED
            #define Errno_WSAECONNREFUSED Errno_ECONNREFUSED
        #endif
        #ifndef Errno_DISTINCT_WSAELOOP
            #define Errno_WSAELOOP Errno_ELOOP
        #endif
        #ifndef Errno_DISTINCT_WSAENAMETOOLONG
            #define Errno_WSAENAMETOOLONG Errno_ENAMETOOLONG
        #endif
        #ifndef Errno_DISTINCT_WSAEHOSTDOWN
            #define Errno_WSAEHOSTDOWN Errno_EHOSTDOWN
        #endif
        #ifndef Errno_DISTINCT_WSAEHOSTUNREACH
            #define Errno_WSAEHOSTUNREACH Errno_EHOSTUNREACH
        #endif
        #ifndef Errno_DISTINCT_WSAENOTEMPTY
            #define Errno_WSAENOTEMPTY Errno_ENOTEMPTY
        #endif
        #ifndef Errno_DISTINCT_WSAEPROCLIM
            #define Errno_WSAEPROCLIM Errno_EPROCLIM
        #endif
        #ifndef Errno_DISTINCT_WSAEUSERS
            #define Errno_WSAEUSERS Errno_EUSERS
        #endif
        #ifndef Errno_DISTINCT_WSAEDQUOT
            #define Errno_WSAEDQUOT Errno_EDQUOT
        #endif
        #ifndef Errno_DISTINCT_WSAESTALE
            #define Errno_WSAESTALE Errno_ESTALE
        #endif
        #ifndef Errno_DISTINCT_WSAEREMOTE
            #define Errno_WSAEREMOTE Errno_EREMOTE
        #endif
    #endif // W32
#endif // Errno_NO_MAPPINGS

enum Errno Errno_getInternal(uint64_t mappingMask);

static inline enum Errno Errno_get()
{
    const uint64_t mappingMask = 0
    #ifndef Errno_DISTINCT_ENOTSUP
        | (((uint64_t)1) <<Errno_Mappings_ENOTSUP)
    #endif
    #ifndef Errno_DISTINCT_EWULDBLOCK
        | (((uint64_t)1) <<Errno_Mappings_EWOULDBLOCK)
    #endif
    #ifndef Errno_DISTINCT_EDEADLK
        | (((uint64_t)1) <<Errno_Mappings_EDEADLK)
    #endif
    #ifdef W32
        #ifndef Errno_DISTINCT_WSAEWOULDBLOCK
            | (((uint64_t)1) <<Errno_Mappings_WSAEWOULDBLOCK)
        #endif
        #ifndef Errno_DISTINCT_WSAEINPROGRESS
            | (((uint64_t)1) <<Errno_Mappings_WSAEINPROGRESS)
        #endif
        #ifndef Errno_DISTINCT_WSAEALREADY
            | (((uint64_t)1) <<Errno_Mappings_WSAEALREADY)
        #endif
        #ifndef Errno_DISTINCT_WSAENOTSOCK
            | (((uint64_t)1) <<Errno_Mappings_WSAENOTSOCK)
        #endif
        #ifndef Errno_DISTINCT_WSAEDESTADDRREQ
            | (((uint64_t)1) <<Errno_Mappings_WSAEDESTADDRREQ)
        #endif
        #ifndef Errno_DISTINCT_WSAEMSGSIZE
            | (((uint64_t)1) <<Errno_Mappings_WSAEMSGSIZE)
        #endif
        #ifndef Errno_DISTINCT_WSAEPROTOTYPE
            | (((uint64_t)1) <<Errno_Mappings_WSAEPROTOTYPE)
        #endif
        #ifndef Errno_DISTINCT_WSAENOPROTOOPT
            | (((uint64_t)1) <<Errno_Mappings_WSAENOPROTOOPT)
        #endif
        #ifndef Errno_DISTINCT_WSAEPROTONOSUPPORT
            | (((uint64_t)1) <<Errno_Mappings_WSAEPROTONOSUPPORT)
        #endif
        #ifndef Errno_DISTINCT_WSAESOCKTNOSUPPORT
            | (((uint64_t)1) <<Errno_Mappings_WSAESOCKTNOSUPPORT)
        #endif
        #ifndef Errno_DISTINCT_WSAEOPNOTSUPP
            | (((uint64_t)1) <<Errno_Mappings_WSAEOPNOTSUPP)
        #endif
        #ifndef Errno_DISTINCT_WSAEPFNOSUPPORT
            | (((uint64_t)1) <<Errno_Mappings_WSAEPFNOSUPPORT)
        #endif
        #ifndef Errno_DISTINCT_WSAEAFNOSUPPORT
            | (((uint64_t)1) <<Errno_Mappings_WSAEAFNOSUPPORT)
        #endif
        #ifndef Errno_DISTINCT_WSAEADDRINUSE
            | (((uint64_t)1) <<Errno_Mappings_WSAEADDRINUSE)
        #endif
        #ifndef Errno_DISTINCT_WSAEADDRNOTAVAIL
            | (((uint64_t)1) <<Errno_Mappings_WSAEADDRNOTAVAIL)
        #endif
        #ifndef Errno_DISTINCT_WSAENETDOWN
            | (((uint64_t)1) <<Errno_Mappings_WSAENETDOWN)
        #endif
        #ifndef Errno_DISTINCT_WSAENETUNREACH
            | (((uint64_t)1) <<Errno_Mappings_WSAENETUNREACH)
        #endif
        #ifndef Errno_DISTINCT_WSAENETRESET
            | (((uint64_t)1) <<Errno_Mappings_WSAENETRESET)
        #endif
        #ifndef Errno_DISTINCT_WSAECONNABORTED
            | (((uint64_t)1) <<Errno_Mappings_WSAECONNABORTED)
        #endif
        #ifndef Errno_DISTINCT_WSAECONNRESET
            | (((uint64_t)1) <<Errno_Mappings_WSAECONNRESET)
        #endif
        #ifndef Errno_DISTINCT_WSAENOBUFS
            | (((uint64_t)1) <<Errno_Mappings_WSAENOBUFS)
        #endif
        #ifndef Errno_DISTINCT_WSAEISCONN
            | (((uint64_t)1) <<Errno_Mappings_WSAEISCONN)
        #endif
        #ifndef Errno_DISTINCT_WSAENOTCONN
            | (((uint64_t)1) <<Errno_Mappings_WSAENOTCONN)
        #endif
        #ifndef Errno_DISTINCT_WSAESHUTDOWN
            | (((uint64_t)1) <<Errno_Mappings_WSAESHUTDOWN)
        #endif
        #ifndef Errno_DISTINCT_WSAETOOMANYREFS
            | (((uint64_t)1) <<Errno_Mappings_WSAETOOMANYREFS)
        #endif
        #ifndef Errno_DISTINCT_WSAETIMEDOUT
            | (((uint64_t)1) <<Errno_Mappings_WSAETIMEDOUT)
        #endif
        #ifndef Errno_DISTINCT_WSAECONNREFUSED
            | (((uint64_t)1) <<Errno_Mappings_WSAECONNREFUSED)
        #endif
        #ifndef Errno_DISTINCT_WSAELOOP
            | (((uint64_t)1) <<Errno_Mappings_WSAELOOP)
        #endif
        #ifndef Errno_DISTINCT_WSAENAMETOOLONG
            | (((uint64_t)1) <<Errno_Mappings_WSAENAMETOOLONG)
        #endif
        #ifndef Errno_DISTINCT_WSAEHOSTDOWN
            | (((uint64_t)1) <<Errno_Mappings_WSAEHOSTDOWN)
        #endif
        #ifndef Errno_DISTINCT_WSAEHOSTUNREACH
            | (((uint64_t)1) <<Errno_Mappings_WSAEHOSTUNREACH)
        #endif
        #ifndef Errno_DISTINCT_WSAENOTEMPTY
            | (((uint64_t)1) <<Errno_Mappings_WSAENOTEMPTY)
        #endif
        #ifndef Errno_DISTINCT_WSAEPROCLIM
            | (((uint64_t)1) <<Errno_Mappings_WSAEPROCLIM)
        #endif
        #ifndef Errno_DISTINCT_WSAEUSERS
            | (((uint64_t)1) <<Errno_Mappings_WSAEUSERS)
        #endif
        #ifndef Errno_DISTINCT_WSAEDQUOT
            | (((uint64_t)1) <<Errno_Mappings_WSAEDQUOT)
        #endif
        #ifndef Errno_DISTINCT_WSAESTALE
            | (((uint64_t)1) <<Errno_Mappings_WSAESTALE)
        #endif
        #ifndef Errno_DISTINCT_WSAEREMOTE
            | (((uint64_t)1) <<Errno_Mappings_WSAEREMOTE)
        #endif
    #endif
    ;

    return Errno_getInternal(mappingMask);
}


char* Errno_strerror(enum Errno err);

static inline char* Errno_getString()
{
    return Errno_strerror(Errno_get());
}

#endif
