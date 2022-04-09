﻿#include "pch.h"
#include "framework.h"
#include "wsa_error.h"

#include <string>
#include <unordered_map>
#include <WinSock2.h>

#ifdef WSAERROR_EXPORTS
#define WSA_STRERR_API extern "C" __declspec(dllexport)
#else
#define WSA_STRERR_API extern "C" __declspec(dllimport)
#endif // WSAERROR_EXPORTS

static std::unordered_map<unsigned long, std::string> errStrMap{
	/* 6, An application attempts to use an event object, but the specified handle is not valid. */
	{WSA_INVALID_HANDLE, "Specified event object handle is invalid"},
	/* 8, An application used a Windows Sockets function that directly maps to a Windows function.The Windows function is indicating a lack of required memory resources. */
	{WSA_NOT_ENOUGH_MEMORY, "Insufficient memory available"},
	/* 87,An application used a Windows Sockets function which directly maps to a Windows function.The Windows function is indicating a problem with one or more parameters. */
	{WSA_INVALID_PARAMETER, "One or more parameters are invalid"},
	/* 995,An overlapped operation was canceled due to the closure of the socket, or the executionof the SIO_FLUSH command in WSAIoctl. */
	{WSA_OPERATION_ABORTED, "Overlapped operation aborted"},
	/* 996,The application has tried to determine the status of an overlapped operation which isnot yet completed. Applications that use WSAGetOverlappedResult (with the fWait flag set toFALSE) in a polling mode to determine when an overlapped operation has completed, get thiserror code until the operation is complete. */
	{WSA_IO_INCOMPLETE, "Overlapped I/O event object not in signaled state"},
	/* 997,The application has initiated an overlapped operation that cannot be completed immediately.A completion indication will be given later when the operation has been completed. */
	{WSA_IO_PENDING, "Overlapped operations will complete later"},
	/* 1004,A blocking operation was interrupted by a call to WSACancelBlockingCall. */
	{WSAEINTR, "Interrupted function call"},
	/* 1009,The file handle supplied is not valid. */
	{WSAEBADF, "File handle is not valid"},
	/* 10013,An attempt was made to access a socket in a way forbidden by its access permissions.An example is using a broadcast address for sendto without broadcast permission being set using setsockopt(SO_BROADCAST). */
	{WSAEACCES, "Permission denied"},
	/* 10014,The system detected an invalid pointer address in attempting to use a pointer argument of a call.This error occurs if an application passes an invalid pointer value, or if the length of the buffer is too small.For instance, if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr). */
	{WSAEFAULT, "Bad address"},
	/* 10022,Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function).In some instances, it also refers to the current state of the socket—for instance, calling accept on a socket that is not listening. */
	{WSAEINVAL, "Invalid argument"},
	/* 10024,Too many open sockets. Each implementation may have a maximum number of socket handles available,either globally, per process, or per thread. */
	{WSAEMFILE, "Too many open files"},
	/* 10035,This error is returned from operations on nonblocking sockets that cannot be completed immediately,for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operationshould be retried later. It is normal for WSAEWOULDBLOCK to be reported as the result from calling connect ona nonblocking SOCK_STREAM socket, since some time must elapse for the connection to be established. */
	{WSAEWOULDBLOCK, "Resource temporarily unavailable"},
	/* 10036,A blocking operation is currently executing. Windows Sockets only allows a single blocking operation—per-taskor thread—to be outstanding, and if any other function call is made (whether or not it references that or any other socket)the function fails with the WSAEINPROGRESS error. */
	{WSAEINPROGRESS, "Operation now in progress"},
	/* 10037,An operation was attempted on a nonblocking socket with an operation already in progress—that is, calling connect a secondtime on a nonblocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already be-en canceled or completed. */
	{WSAEALREADY, "Operation already in progress"},
	/* 10038,An operation was attempted on something that is not a socket. Either the socket handle parameter did not reference a validsocket, or for select, a member of an fd_set was not valid. */
	{WSAENOTSOCK, "Socket operation on nonsocket"},
	/* 10039,A required address was omitted from an operation on a socket. For example, this error is returned if sendto is called withthe remote address of ADDR_ANY. */
	{WSAEDESTADDRREQ, "Destination address required"},
	/* 10040,A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the bufferused to receive a datagram was smaller than the datagram itself. */
	{WSAEMSGSIZE, "Message too long"},
	/* 10041,A protocol was specified in the socket function call that does not support the semantics of the socket type requested.For example, the ARPA Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM. */
	{WSAEPROTOTYPE, "Protocol wrong type for socket"},
	/* 10042,An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call. */
	{WSAENOPROTOOPT, "Bad protocol option"},
	/* 10043,The requested protocol has not been configured into the system, or no implementation for it exists. For example,a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol. */
	{WSAEPROTONOSUPPORT, "Protocol not supported"},
	/* 10044,The support for the specified socket type does not exist in this address family. For example,the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all. */
	{WSAESOCKTNOSUPPORT, "Socket type not supported"},
	/* 10045,The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptorto a socket that cannot support this operation is trying to accept a connection on a datagram socket. */
	{WSAEOPNOTSUPP, "Operation not supported"},
	/* 10046,The protocol family has not been configured into the system or no implementation for it exists. This message has a-tions that return one of these messages also specify WSAEAFNOSUPPORT. */
	{WSAEPFNOSUPPORT, "Protocol family not supported"},
	/* 10047, An address incompatible with the requested protocol was used. All sockets are created with an associated addressfamily (that is, AF_INET for Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). This error is returnedif an incorrect protocol is explicitly requested in the socket call, or if an address of the wrong family is used for a socket,for example, in sendto. */
	{WSAEAFNOSUPPORT, "Address family not supported by protocol family"},
	/* 10048,Typically, only one usage of each socket address (protocol/IP address/port) is permitted. This error occurs if anapplication attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socketthat was not closed properly, or one that is still in the process of closing. For server applications that need to bindmultiple sockets to the same port number, consider using setsockopt (SO_REUSEADDR). Client applications usually need notcall bind at all—connect chooses an unused port automatically. When bind is called with a wildcard address (involving ADDR_ANY),a WSAEADDRINUSE error could be delayed until the specific address is committed. This could happen with a call to another function later,including connect, listen, WSAConnect, or WSAJoinLeaf. */
	{WSAEADDRINUSE, "Address already in use"},
	/* 10049,The requested address is not valid in its context. This normally results from an attempt to bind to an address that is not valid for the local computer. This can also result from connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for a remote computer (for example, address or port 0). */
	{WSAEADDRNOTAVAIL, "Cannot assign requested address"},
	/* 10050,A socket operation encountered a dead network. This could indicate a serious failure of the network system (that is, the protocol stack that the Windows Sockets DLL runs over), the network interface, or the local network itself. */
	{WSAENETDOWN, "Network is down"},
	/* 10051,A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host. */
	{WSAENETUNREACH, "Network is unreachable"},
	/* 10052,The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. It can also be returned by setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that has already failed. */
	{WSAENETRESET, "Network dropped connection on reset"},
	/* 10053,An established connection was aborted by the software in your host computer, possibly due to a data transmission time-out or protocol error. */
	{WSAECONNABORTED, "Software caused connection abort"},
	/* 10054,An existing connection was forcibly closed by the remote host. This normally results if the peer application on the remote host is suddenly stopped, the host is rebooted, the host or remote network interface is disabled, or the remote host uses a hard close (see setsockopt for more information on the SO_LINGER option on the remote socket). This error may also result if a connection was broken due to keep-alive activity detecting a failure while one or more operations are in progress. Operations that were in progress fail with WSAENETRESET. Subsequent operations fail with WSAECONNRESET. */
	{WSAECONNRESET, "Connection reset by peer"},
	/* 10055,An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full. */
	{WSAENOBUFS, "No buffer space available"},
	/* 10056,A connect request was made on an already-connected socket. Some implementations also return this error if sendto is called on a connected SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter in sendto is ignored) although other implementations treat this as a legal occurrence. */
	{WSAEISCONN, "Socket is already connected"},
	/* 10057,A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using sendto) no address was supplied. Any other type of operation might also return this error—for example, setsockopt setting SO_KEEPALIVE if the connection has been reset. */
	{WSAENOTCONN, "Socket is not connected"},
	/* 10058,A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. By calling shutdown a partial close of a socket is requested, which is a signal that sending or receiving, or both have been discontinued. */
	{WSAESHUTDOWN, "Cannot send after socket shutdown"},
	/* 10059,Too many references to some kernel object. */
	{WSAETOOMANYREFS, "Too many references"},
	/* 10060,A connection attempt failed because the connected party did not properly respond after a period of time, or the established connection failed because the connected host has failed to respond. */
	{WSAETIMEDOUT, "Connection timed out"},
	/* 10061,No connection could be made because the target computer actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host—that is, one with no server application running. */
	{WSAECONNREFUSED, "Connection refused"},
	/* 10062,Cannot translate a name. */
	{WSAELOOP, "Cannot translate name"},
	/* 10063,A name component or a name was too long. */
	{WSAENAMETOOLONG, "Name too long"},
	/* 10064,A socket operation failed because the destination host is down. A socket operation encountered a dead host. Networking activity on the local host has not been initiated. These conditions are more likely to be indicated by the error WSAETIMEDOUT. */
	{WSAEHOSTDOWN, "Host is down"},
	/* 10065,A socket operation was attempted to an unreachable host. See WSAENETUNREACH. */
	{WSAEHOSTUNREACH, "No route to host"},
	/* 10066,Cannot remove a directory that is not empty.*/
	{WSAENOTEMPTY, "Directory not empty"},
	/* 10067,A Windows Sockets implementation may have a limit on the number of applications that can use it simultaneously. WSAStartup may fail with this error if the limit has been reached. */
	{WSAEPROCLIM, "Too many processes"},
	/* 10068,Ran out of user quota. */
	{WSAEUSERS, "User quota exceeded"},
	/* 10069,Ran out of disk quota. */
	{WSAEDQUOT, "Disk quota exceeded"},
	/* 10070,The file handle reference is no longer available. */
	{WSAESTALE, "Stale file handle reference"},
	/* 10071,The file handle reference is no longer available. */
	{WSAEREMOTE, "Stale file handle reference"},
	/* 10091,This error is returned by WSAStartup if the Windows Sockets implementation cannot function at this time because the underlying system it uses to provide network services is currently unavailable. */
	{WSASYSNOTREADY, "Network subsystem is unavailable"},
	/* 10092,The current Windows Sockets implementation does not support the Windows Sockets specification version requested by the application. Check that no old Windows Sockets DLL files are being accessed. */
	{WSAVERNOTSUPPORTED, "Winsock.dll version out of range"},
	/* 10093,Either the application has not called WSAStartup or WSAStartup failed.  */
	{WSANOTINITIALISED, "Successful WSAStartup not yet performed"},
	/* 10101,Returned by WSARecv and WSARecvFrom to indicate that the remote party has initiated a graceful shutdown sequence. */
	{WSAEDISCON, "Graceful shutdown in progress"},
	/* 10102,No more results can be returned by the WSALookupServiceNext function. */
	{WSAENOMORE, "No more results"},
	/* 10103,A call to the WSALookupServiceEnd function was made while this call was still processing. The call has been canceled. */
	{WSAECANCELLED, "Call has been canceled"},
	/* 10104,The service provider procedure call table is invalid. A service provider returned a bogus procedure table to Ws2_32.dll. This is usually caused by one or more of the function pointers being NULL.*/
	{WSAEINVALIDPROCTABLE, "Procedure call table is invalid"},
	/* 10105,The requested service provider is invalid. This error is returned by the WSCGetProviderInfo and WSCGetProviderInfo32 functions if the protocol entry specified could not be found. This error is also returned if the service provider returned a version number other than 2.0. */
	{WSAEINVALIDPROVIDER, "Service provider is invalid"},
	/* 10106,The requested service provider could not be loaded or initialized. This error is returned if either a service provider's DLL could not be loaded (LoadLibrary failed) or the provider's WSPStartup or NSPStartup function failed. */
	{WSAEPROVIDERFAILEDINIT, "Service provider failed to initialize"},
	/* 10107,A system call that should never fail has failed. This is a generic error code, returned under various conditions.Returned when a system call that should never fail does fail. For example, if a call to WaitForMultipleEvents fails or one of the registry functions fails trying to manipulate the protocol/namespace catalogs.Returned when a provider does not return SUCCESS and does not provide an extended error code. Can indicate a service provider implementation error. */
	{WSASYSCALLFAILURE, "System call failure"},
	/* 10108,No such service is known. The service cannot be found in the specified name space. */
	{WSASERVICE_NOT_FOUND, "Service not found"},
	/* 10109,The specified class was not found. */
	{WSATYPE_NOT_FOUND, "No more results"},
	/* 10110,No more results can be returned by the WSALookupServiceNext function. */
	{WSA_E_NO_MORE, "No more results"},
	/* 10111,A call to the WSALookupServiceEnd function was made while this call was still processing. The call has been canceled. */
	{WSA_E_CANCELLED, "Call was canceled"},
	/* 10112,A database query failed because it was actively refused. */
	{WSAEREFUSED, "Database query was refused"},
	/* 11001,No such host is known. The name is not an official host name or alias, or it cannot be found in the database(s) being queried. This error may also be returned for protocol and service queries, and means that the specified name could not be found in the relevant database. */
	{WSAHOST_NOT_FOUND, "Host not found"},
	/* 11002,This is usually a temporary error during host name resolution and means that the local server did not receive a response from an authoritative server. A retry at some time later may be successful. */
	{WSATRY_AGAIN, "Nonauthoritative host not found"},
	/* 11003,This indicates that some sort of nonrecoverable error occurred during a database lookup. This may be because the database files (for example, BSD-compatible HOSTS, SERVICES, or PROTOCOLS files) could not be found, or a DNS request was returned by the server with a severe error. */
	{WSANO_RECOVERY, "This is a nonrecoverable error"},
	/* 11004,The requested name is valid and was found in the database, but it does not have the correct associated data being resolved for. The usual example for this is a host name-to-address translation attempt (using gethostbyname or WSAAsyncGetHostByName) which uses the DNS (Domain Name Server). An MX record is returned but no A record—indicating the host itself exists, but is not directly reachable. */
	{WSANO_DATA, "Valid name, no data record of requested type"},
	/* */
	{WSA_QOS_RECEIVERS, "QoS receivers"},
	/* */
	{WSA_QOS_SENDERS, "QoS senders"},
	/* */
	{WSA_QOS_NO_SENDERS, "No QoS senders"},
	/* */
	{WSA_QOS_NO_RECEIVERS, "QoS no receivers"},
	/* */
	{WSA_QOS_REQUEST_CONFIRMED, "QoS request confirmed"},
	/* */
	{WSA_QOS_ADMISSION_FAILURE, "QoS admission error"},
	/* */
	{WSA_QOS_POLICY_FAILURE, "QoS policy failure"},
	/* */
	{WSA_QOS_BAD_STYLE, "QoS bad style"},
	/* */
	{WSA_QOS_BAD_OBJECT, "QoS bad object"},
	/* */
	{WSA_QOS_TRAFFIC_CTRL_ERROR, "QoS traffic control error"},
	/* */
	{WSA_QOS_GENERIC_ERROR, "QoS generic error"},
	/* */
	{WSA_QOS_ESERVICETYPE, "QoS service type error"},
	/* */
	{WSA_QOS_EFLOWSPEC, "QoS flowspec error"},
	/* */
	{WSA_QOS_EPROVSPECBUF, "Invalid QoS provider buffer"},
	/* */
	{WSA_QOS_EFILTERSTYLE, "Invalid QoS filter style"},
	/* 11020,An invalid QoS filter type was used. */
	{WSA_QOS_EFILTERTYPE, "Invalid QoS filter type"},
	/* 11021,An incorrect number of QoS FILTERSPECs were specified in the FLOWDESCRIPTOR. */
	{WSA_QOS_EFILTERCOUNT, "Incorrect QoS filter count"},
	/* 11022,An object with an invalid ObjectLength field was specified in the QoS provider-specific buffer. */
	{WSA_QOS_EOBJLENGTH, "Invalid QoS object length"},
	/* 11023,An incorrect number of flow descriptors was specified in the QoS structure. */
	{WSA_QOS_EFLOWCOUNT, "Incorrect QoS flow count"},
	/* 11024,An unrecognized object was found in the QoS provider-specific buffer. */
	{WSA_QOS_EUNKOWNPSOBJ, "Unrecognized QoS object"},
	/* 11025,An invalid policy object was found in the QoS provider-specific buffer. */
	{WSA_QOS_EPOLICYOBJ, "Invalid QoS policy object"},
	/* 11026,An invalid QoS flow descriptor was found in the flow descriptor list. */
	{WSA_QOS_EFLOWDESC, "Invalid QoS flow descriptor"},
	/* 11027,An invalid or inconsistent flowspec was found in the QoS provider-specific buffer. */
	{WSA_QOS_EPSFLOWSPEC, "Invalid QoS provider-specific flowspec"},
	/* 11028,An invalid FILTERSPEC was found in the QoS provider-specific buffer. */
	{WSA_QOS_EPSFILTERSPEC, "Invalid QoS provider-specific filterspec"},
	/* 11029,An invalid shape discard mode object was found in the QoS provider-specific buffer. */
	{WSA_QOS_ESDMODEOBJ, "Invalid QoS shape discard mode object"},
	/* 11030,An invalid shaping rate object was found in the QoS provider-specific buffer. */
	{WSA_QOS_ESHAPERATEOBJ, "Invalid QoS shaping rate object"},
	/* 11031,A reserved policy element was found in the QoS provider-specific buffer. */
	{WSA_QOS_RESERVED_PETYPE, "Reserved policy QoS element type"}
};

WSA_STRERR_API const char* WSAStrError(unsigned long ulErrno)
{
	auto value = errStrMap.find(ulErrno);
	if (value != errStrMap.end())
	{
		return value->second.c_str();
	}
	else
	{
		return "Unknown error";
	}
}
