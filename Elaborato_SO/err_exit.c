/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file err_exit.c
/// @brief functions for errno errors management

#include "err_exit.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/// @brief prints an error message, removes IPC resources, and terminates the program with error
void errExit(const char *message){
    perror(message);
    printf(BOLDRED"\n> forced ipc removal <\n" RESET);

    // show remove ipc details
    system("ipcrm --all --verbose");
    printf(BOLDRED"\n> forced file removal <\n" RESET);

    // remove FIFO, pid files and temp files
    system("rm -f -r FIFO pid_countDown_.txt pid_readToken_.txt F4ServerRun F4Client1Run F4Client2Run");
    exit(1);
}

/// @brief prints errno message details
void errnoCheck(int errnoValue) {
    if (errnoValue == E2BIG) {
        errExit("> errno E2BIG [argument list too long.] <\n");
    }

    if (errnoValue == EACCES) {
        errExit("> errno EACCES [permission denied.] <\n");
    }

    if (errnoValue == EADDRINUSE) {
        errExit("> errno EADDRINUSE [address already in use] <\n");
    }

    if (errnoValue == EADDRNOTAVAIL) {
        errExit("> errno EADDRNOTAVAIL [address not available] <\n");
    }

    if (errnoValue == EAFNOSUPPORT) {
        errExit("> errno EAFNOSUPPORT [address family not supported] <\n");
    }

    if (errnoValue == EAGAIN) {
        errExit("> errno EAGAIN [resource temporarily unavailable"
                " (may be the same value as EWOULDBLOCK)] <\n");
    }

    if (errnoValue == EALREADY) {
        errExit("> errno EALREADY [connection already in progress] <\n");
    }

    if (errnoValue == EBADE) {
        errExit("> errno EBADE [invalid exchange] <\n");
    }

    if (errnoValue == EBADF) {
        errExit("> errno EBADF [bad file descriptor] <\n");
    }

    if (errnoValue == EBADFD) {
        errExit("> errno EBADFD [file descriptor in bad state] <\n");
    }

    if (errnoValue == EBADMSG) {
        errExit("> errno EBADMSG [bad message] <\n");
    }

    if (errnoValue == EBADR) {
        errExit("> errno EBADR [invalid request descriptor] <\n");
    }

    if (errnoValue == EBADRQC) {
        errExit("> errno EBADRQC [invalid request code] <\n");
    }

    if (errnoValue == EBADSLT) {
        errExit("> errno EBADSLT [invalid slot] <\n");
    }

    if (errnoValue == EBUSY) {
        errExit("> errno EBUSY [device or resource busy] <\n");
    }

    if (errnoValue == ECANCELED) {
        errExit("> errno ECANCELED [operation canceled] <\n");
    }

    if (errnoValue == ECHILD) {
        errExit("> errno ECHILD [no child process] <\n");
    }

    if (errnoValue == ECHRNG) {
        errExit("> errno ECHRNG [channel number out of range] <\n");
    }

    if (errnoValue == ECOMM) {
        errExit("> errno ECOMM [communication error on sand] <\n");
    }

    if (errnoValue == ECONNABORTED) {
        errExit("> errno ECONNABORTED [connection aborted] <\n");
    }

    if (errnoValue == ECONNREFUSED) {
        errExit("> errno ECONNREFUSED [connection refused] <\n");
    }

    if (errnoValue == ECONNRESET) {
        errExit("> errno ECONNRESET [connection reset] <\n");
    }

    if (errnoValue == EDEADLK) {
        errExit("> errno EDEADLK [resource deadlock avoided] <\n");
    }

    //da riguardare
    if (errnoValue == EDEADLOCK) {
        errExit("> errno EDEADLOCK [resource deadlock avoided] <\n");
    }

    if (errnoValue == EDESTADDRREQ) {
        errExit("> errno EDESTADDRREQ [destination address required] <\n");
    }

    if (errnoValue == EDOM) {
        errExit("> errno EDOM [mathematics argument out of domain of function] <\n");
    }

    if (errnoValue == EDQUOT) {
        errExit("> errno EDQUOT [disk quota exceeded] <\n");
    }

    if (errnoValue == EEXIST) {
        errExit("> errno EEXIST [file exists] <\n");
    }

    if (errnoValue == EFAULT) {
        errExit("> errno EFAULT [bad address] <\n");
    }

    if (errnoValue == EFBIG) {
        errExit("> errno EFBIG [file too large] <\n");
    }

    if (errnoValue == EHOSTDOWN) {
        errExit("> errno EHOSTDOWN [host is down] <\n");
    }

    if (errnoValue == EHOSTUNREACH) {
        errExit("> errno EHOSTUNREACH [host is unreachable] <\n");
    }

    if (errnoValue == EHWPOISON) {
        errExit("> errno EHWPOISON [memory page has hardware error] <\n");
    }

    if (errnoValue == EIDRM) {
        errExit("> errno EIDRM [identifier removed] <\n");
    }

    if (errnoValue == EILSEQ) {
        errExit("> errno EILSEQ [invalid or incomplete multibyte or wide character] <\n");
    }

    if (errnoValue == EINPROGRESS) {
        errExit("> errno EINPROGRESS [operation in progress] <\n");
    }

    if (errnoValue == EINTR) {
        errExit("> errno EINTR [interrupted function call] <\n");
    }

    if (errnoValue == EINVAL) {
        errExit("> errno EINVAL [invalid argument] <\n");
    }

    if (errnoValue == EIO) {
        errExit("> errno EIO [input/output error] <\n");
    }

    if (errnoValue == EISCONN) {
        errExit("> errno EISCONN [socket is connected] <\n");
    }

    if (errnoValue == EISDIR) {
        errExit("> errno EISDIR [is a directory] <\n");
    }

    if (errnoValue == EISNAM) {
        errExit("> errno EISNAM [is a named type file] <\n");
    }

    if (errnoValue == EKEYEXPIRED) {
        errExit("> errno EKEYEXPIRED [key has expired] <\n");
    }

    if (errnoValue == EKEYREJECTED) {
        errExit("> errno EKEYREJECTED [key was rejected] <\n");
    }

    if (errnoValue == EKEYREVOKED) {
        errExit("> errno EKEYREVOKED [key has been revoked] <\n");
    }

    if (errnoValue == EL2HLT) {
        errExit("> errno EL2HLT [level 2 halted] <\n");
    }

    if (errnoValue == EL2NSYNC) {
        errExit("> errno EL2NSYNC [level 2 not synchronised] <\n");
    }

    if (errnoValue == EL3HLT) {
        errExit("> errno EL3HLT [level 3 halted] <\n");
    }

    if (errnoValue == EL3RST) {
        errExit("> errno EL3RST [level 3 reset] <\n");
    }

    if (errnoValue == ELIBACC) {
        errExit("> errno ELIBACC [cannot access a needed shared library] <\n");
    }

    if (errnoValue == ELIBBAD) {
        errExit("> errno ELIBBAD [accessing a corrupted shared library] <\n");
    }

    if (errnoValue == ELIBMAX) {
        errExit("> errno ELIBMAX [attempting to link in too many shared libraries] <\n");
    }

    if (errnoValue == ELIBSCN) {
        errExit("> errno ELIBSCN [.lib section in a.out corrupted] <\n");
    }

    if (errnoValue == ELIBEXEC) {
        errExit("> errno ELIBEXEC [cannot exec a shared library directly] <\n");
    }

    if (errnoValue == ELNRNG) {
        errExit("> errno ELNRNG [link number out of range] <\n");
    }

    if (errnoValue == ELOOP) {
        errExit("> errno ELOOP [too many levels of symbolic links] <\n");
    }

    if (errnoValue == ELOOP) {
        errExit("> errno ELOOP [too many levels of symbolic links] <\n");
    }

    if (errnoValue == EMEDIUMTYPE) {
        errExit("> errno EMEDIUMTYPE [wrong medium type] <\n");
    }

    if (errnoValue == EMFILE) {
        errExit("> errno EMFILE [too many open files."
                " Commonly caused by exceeding the RLIMIT_NOFILE resource limit described in getrlimit(2)."
                " Can also be caused by exceeding the limit specified in /proc/sys/fs/nr_open] <\n");
    }

    if (errnoValue == EMLINK) {
        errExit("> errno EMLINK [too many links] <\n");
    }

    if (errnoValue == EMSGSIZE) {
        errExit("> errno EMSGSIZE [message too long] <\n");
    }

    if (errnoValue == EMULTIHOP) {
        errExit("> errno EMULTIHOP [multihop attempted] <\n");
    }

    if (errnoValue == ENAMETOOLONG) {
        errExit("> errno ENAMETOOLONG [file name too long] <\n");
    }

    if (errnoValue == ENETDOWN) {
        errExit("> errno ENETDOWN [network is down] <\n");
    }

    if (errnoValue == ENETRESET) {
        errExit("> errno ENETRESET [connection aborted by network] <\n");
    }

    if (errnoValue == ENETUNREACH) {
        errExit("> errno ENETUNREACH [network unreachable] <\n");
    }

    if (errnoValue == ENFILE) {
        errExit("> errno ENFILE [too many open files in system."
                " On Linux, this is probably a result of encountering the /proc/sys/fs/file-max limit (see proc(5))] <\n");
    }

    if (errnoValue == ENOANO) {
        errExit("> errno ENOANO [no anode] <\n");
    }

    if (errnoValue == ENOBUFS) {
        errExit("> errno ENOBUFS [no buffer space available] <\n");
    }

    if (errnoValue == ENODATA) {
        errExit("> errno ENODATA [the named attribute does not exist,"
                " or the process has no access to thi attribute; see xattr(7)] <\n");
    }

    if (errnoValue == ENODEV) {
        errExit("> errno ENODEV [no such device] <\n");
    }

    if (errnoValue == ENOENT) {
        errExit("> errno ENOENT [no such file or directory] <\n");
    }

    if (errnoValue == ENOEXEC) {
        errExit("> errno ENOEXEC [exec format error] <\n");
    }

    if (errnoValue == ENOKEY) {
        errExit("> errno ENOKEY [required key not available] <\n");
    }

    if (errnoValue == ENOLCK) {
        errExit("> errno ENOLCK [no locks available] <\n");
    }

    if (errnoValue == ENOMEDIUM) {
        errExit("> errno ENOMEDIUM [no medium found] <\n");
    }

    if (errnoValue == ENOMEM) {
        errExit("> errno ENOMEM [not enough space/cannot allocate memory] <\n");
    }

    if (errnoValue == ENOMSG) {
        errExit("> errno ENOMSG [no message of the desired type] <\n");
    }

    if (errnoValue == ENONET) {
        errExit("> errno ENONET [machine is not on the network] <\n");
    }

    if (errnoValue == ENOPKG) {
        errExit("> errno ENOPKG [package not installed] <\n");
    }

    if (errnoValue == ENOPROTOOPT) {
        errExit("> errno ENOPROTOOPT [protocol not available] <\n");
    }

    if (errnoValue == ENOSPC) {
        errExit("> errno ENOSPC [no space left on device] <\n");
    }

    if (errnoValue == ENOSR) {
        errExit("> errno ENOSR [no STREAM resources] <\n");
    }

    if (errnoValue == ENOSTR) {
        errExit("> errno ENOSTR [not a STREAM] <\n");
    }

    if (errnoValue == ENOSYS) {
        errExit("> errno ENOSYS [function not implemented] <\n");
    }

    if (errnoValue == ENOTBLK) {
        errExit("> errno ENOTBLK [block device required] <\n");
    }

    if (errnoValue == ENOTCONN) {
        errExit("> errno ENOTCONN [the socket is not connected] <\n");
    }

    if (errnoValue == ENOTDIR) {
        errExit("> errno ENOTDIR [not a directory] <\n");
    }

    if (errnoValue == ENOTEMPTY) {
        errExit("> errno ENOTEMPTY [directory not empty] <\n");
    }

    if (errnoValue == ENOTRECOVERABLE) {
        errExit("> errno ENOTRECOVERABLE [state not recoverable] <\n");
    }

    if (errnoValue == ENOTSOCK) {
        errExit("> errno ENOTSOCK [not a socket] <\n");
    }

    if (errnoValue == ENOTSUP) {
        errExit("> errno ENOTSUP [operation not supported] <\n");
    }

    if (errnoValue == ENOTTY) {
        errExit("> errno ENOTTY [inappropriate I/O control operation] <\n");
    }

    if (errnoValue == ENOTUNIQ) {
        errExit("> errno ENOTUNIQ [name not unique on network] <\n");
    }

    if (errnoValue == ENXIO) {
        errExit("> errno ENXIO [no such device or address] <\n");
    }

    if (errnoValue == EOPNOTSUPP) {
        errExit("> errno EOPNOTSUPP [operation not supported on socket] <\n");
    }

    if (errnoValue == EOVERFLOW) {
        errExit("> errno EOVERFLOW [value too large to be stored in data type] <\n");
    }

    if (errnoValue == EOWNERDEAD) {
        errExit("> errno EOWNERDEAD [owner died] <\n");
    }

    if (errnoValue == EPERM) {
        errExit("> errno EPERM [operation not permitted] <\n");
    }

    if (errnoValue == EPFNOSUPPORT) {
        errExit("> errno EPFNOSUPPORT [protocol family not supported] <\n");
    }

    if (errnoValue == EPIPE) {
        errExit("> errno EPIPE [broken pipe] <\n");
    }

    if (errnoValue == EPROTO) {
        errExit("> errno EPROTO [protocol error] <\n");
    }

    if (errnoValue == EPROTONOSUPPORT) {
        errExit("> errno EPROTONOTSUPPORT [protocol not supported] <\n");
    }

    if (errnoValue == EPROTOTYPE) {
        errExit("> errno EPROTOTYPE [protocol wrong for type for socket] <\n");
    }

    if (errnoValue == ERANGE) {
        errExit("> errno ERANGE [result too large] <\n");
    }

    if (errnoValue == EREMCHG) {
        errExit("> errno EREMCHG [remote address changed] <\n");
    }

    if (errnoValue == EREMOTE) {
        errExit("> errno EREMOTE [object is remote] <\n");
    }

    if (errnoValue == EREMOTEIO) {
        errExit("> errno EREMOTEIO [remote I/O error] <\n");
    }

    if (errnoValue == ERESTART) {
        errExit("> errno ERESTART [interrupted system call should be restarted] <\n");
    }

    if (errnoValue == ERFKILL) {
        errExit("> errno ERFKILL [operation not possible due to RF-kill] <\n");
    }

    if (errnoValue == EROFS) {
        errExit("> errno EROFS [read-only filesystem] <\n");
    }

    if (errnoValue == ESHUTDOWN) {
        errExit("> errno ESHUTDOWN [cannot send after transport endpoint shutdown] <\n");
    }

    if (errnoValue == ESPIPE) {
        errExit("> errno ESPIPE [invalid seek] <\n");
    }

    if (errnoValue == ESOCKTNOSUPPORT) {
        errExit("> errno ESOCKTNOSUPPORT [socket type not supported] <\n");
    }

    if (errnoValue == ESRCH) {
        errExit("> errno ESRCH [no such process] <\n");
    }

    if (errnoValue == ESTALE) {
        errExit("> errno ESTALE [stale file handle."
                " This error can occur for NFS and for other filesystems] <\n");
    }

    if (errnoValue == ESTRPIPE) {
        errExit("> errno ESTRPIPE [streams pipe error] <\n");
    }

    if (errnoValue == ETIME) {
        errExit("> errno ETIME [time expired] <\n");
    }

    if (errnoValue == ETIMEDOUT) {
        errExit("> errno ETIMEDOUT [connection timed out] <\n");
    }

    if (errnoValue == ETOOMANYREFS) {
        errExit("> errno ETOOMANYREFS [too many references: cannot splice] <\n");
    }

    if (errnoValue == ETXTBSY) {
        errExit("> errno ETXTBSY [text file busy] <\n");
    }

    if (errnoValue == EUCLEAN) {
        errExit("> errno EUCLEAN [structure needs cleaning] <\n");
    }

    if (errnoValue == EUNATCH) {
        errExit("> errno EUNATCH [protocol driver not attached] <\n");
    }

    if (errnoValue == EUSERS) {
        errExit("> errno EUSERS [too many users] <\n");
    }

    if (errnoValue == EWOULDBLOCK) {
        errExit("> errno EWOULDBLOCK [operation would block (maybe same values of EAGAIN)] <\n");
    }

    if (errnoValue == EXDEV) {
        errExit("> errno EXDEV [improper link] <\n");
    }

    if (errnoValue == EXFULL) {
        errExit("> errno EXFULL [exchange full] <\n");
    }
}