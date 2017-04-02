#pragma once
//mainly berkely sockets


//source wikipedia
//On SVR4 systems use of close() may discard data. The use of shutdown() or SO_LINGER may be required on these systems to guarantee delivery of all data.
	//note you may close directly without shutdown, bc you would need an ack anyway if you want confirm delivery.


#ifdef _WIN32
//note it's not thread safe, you might want to add thread safety with mutex
//note an alternative is to call WSAStartup multiple times directly, it should have an internal counter for same num of times calling WSACleanup (still NOT thread-safe)
static unsigned numofmodules_using_winsockdll;
#define CALL_WSAStartup_IF_NEEDED_N_INC_IF_NOERRORs(intRet,caseFail,caseSucceed){\
	if(numofmodules_using_winsockdll){++numofmodules_using_winsockdll;caseSucceed}else{\
		WSADATA wsaData;intRet=WSAStartup(MAKEWORD(2,2), &wsaData);\
		if(intRet){caseFail}else{caseSucceed}\
	}\
}

#define CALL_WSACleanup_IF_NEEDED_N_DEC(intRet,caseErr,caseSucceed){\
	--numofmodules_using_winsockdll;/*//? cleanup fail or succeed doesn't matter for winsock 2.2 bc msdn says so?*/\
	if(numofmodules_using_winsockdll){caseSucceed}else{\
		intRet=WSACleanup();\
		if(intRet){caseErr}else{caseSucceed}\
	}\
}

typedef SOCKET portable_rt_socket_t;
typedef u_long portable_rt_addr_int_t;//unsigned long in struct, should be the same anyway
typedef u_short portable_rt_port_int_t;//unsigned short in struct, should be the same anyway
#define MAKE_PORT_WITH_INTEGERd(id,i) portable_rt_port_int_t id = htons(i);

#define BERKELEY_SO_CLOSE closesocket

#define PORTABLE_RT_SO_GET_ERROR_INT_OR_ERRNO WSAGetLastError()

#define PORTABLE_RT_SO_INITs(intRetBuf,caseFail,caseSucceed) CALL_WSAStartup_IF_NEEDED_N_INC_IF_NOERRORs(intRetBuf,caseFail,caseSucceed)
//note this always quits successfully, although non-zero return value may indicate something unexpected.
#define PORTABLE_RT_SO_QUITs(intRetBuf,anyErr) {intRetBuf=0; CALL_WSACleanup_IF_NEEDED_N_DEC(anyErr,,)}//there are too many cases: caseSucceed,caseSucceedWithErr,caseFailWithErr, and maybe even caseFailWithoutErr. So forget about adding cases in macro argv.

#else
//include <sys/socket.h>

typedef int portable_rt_socket_t;
typedef uint32_t portable_rt_addr_int_t;
typedef uint16_t portable_rt_port_int_t;//same as in_port_t?
#define MAKE_PORT_WITH_INTEGERd(id,i) portable_rt_port_int_t id = htons(i);
#define INVALID_SOCKET -1//winsock2 naming
#define SOCKET_ERROR -1//winsock2 naming

#ifdef __OS2__
#define BERKELEY_SO_CLOSE soclose
#else
#define BERKELEY_SO_CLOSE close
#endif

#define PORTABLE_RT_SO_GET_ERROR_INT_OR_ERRNO errno

//todo add things like signal()?
#define PORTABLE_RT_SO_INITs(intRetBuf,caseFail,caseSucceed) {intRetBuf=0; caseSucceed}
#define PORTABLE_RT_SO_QUITs(intRetBuf,anyErr) {intRetBuf=0; anyErr=0;}

#endif

#ifdef _WIN32
#define memset_sockaddr_in_for_LISTEN_WITH_NI_AND_PORTu(sock_addr)
#else
#define memset_sockaddr_in_for_LISTEN_WITH_NI_AND_PORTu(sock_addr) memset(&sock_addr, 0, sizeof(sock_addr));//what platform requires this line? maybe none? unsure?
#endif
#define LISTEN_WITH_NI_AND_PORTu(ui,listensocket,sin_addr_s_addr,sa_sin_port,caseInvalidSocket,caseBindErr,caseListenErr,caseSucceed){\
	listensocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);\
	if(listensocket==INVALID_SOCKET){caseInvalidSocket}else{\
		struct sockaddr_in ui##sock_addr;\
		memset_sockaddr_in_for_LISTEN_WITH_NI_AND_PORTu(ui##sock_addr)\
		ui##sock_addr.sin_family = AF_INET;\
		ui##sock_addr.sin_addr.s_addr = sin_addr_s_addr;\
		ui##sock_addr.sin_port = sa_sin_port;\
		/*you should cast sockaddr_in to sockaddr when passing to bind, as convention (remember casting doesn't break strict aliasing and only dereference can break it? you may not know how bind() really works. e.g. maybe it's used as if it's a char array, and member sa_data is char[] in the first place)*/\
		if(bind(listensocket,(struct sockaddr *)&ui##sock_addr,sizeof ui##sock_addr)==SOCKET_ERROR){caseBindErr}else{\
			if(listen(listensocket,SOMAXCONN)==SOCKET_ERROR){caseListenErr}else{caseSucceed}\
		}\
	}\
}

#define LISTEN_INADDR_LOOPBACK_WITH_PORT_IN_HOST_BYTE_ORDERu(ui,listensocket,PORT,caseInvalidSocket,caseBindErr,caseListenErr,caseSucceed) LISTEN_WITH_NI_AND_PORTu(ui,listensocket,htonl(INADDR_LOOPBACK),htons(PORT),caseInvalidSocket,caseBindErr,caseListenErr,caseSucceed)
//htonl may be unnecessary bc it's noop for inaddr_any, but still add it bc examples do so
#define LISTEN_INADDR_ANY_WITH_PORT_IN_HOST_BYTE_ORDERu(ui,listensocket,PORT,caseInvalidSocket,caseBindErr,caseListenErr,caseSucceed) LISTEN_WITH_NI_AND_PORTu(ui,listensocket,htonl(INADDR_ANY),htons(PORT),caseInvalidSocket,caseBindErr,caseListenErr,caseSucceed)

#define CONNECT_WITH_ADDR_AND_PORTu(ui,sock,sin_addr_s_addr,sa_sin_port,caseInvalidSocket,caseConnectErr,caseSucceed){\
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);\
	if(sock==INVALID_SOCKET){caseInvalidSocket}else{\
		struct sockaddr_in ui##sock_addr;\
		memset_sockaddr_in_for_LISTEN_WITH_NI_AND_PORTu(ui##sock_addr)\
		ui##sock_addr.sin_family = AF_INET;\
		ui##sock_addr.sin_addr.s_addr = sin_addr_s_addr;\
		ui##sock_addr.sin_port = sa_sin_port;\
		if(connect(sock,(struct sockaddr *)&ui##sock_addr,sizeof ui##sock_addr)==SOCKET_ERROR){caseConnectErr}else{caseSucceed}\
	}\
}

#define CONNECT_INADDR_LOOPBACK_WITH_PORT_IN_HOST_BYTE_ORDERu(ui,sock,portInHostByteOrder,caseInvalidSocket,caseConnectErr,caseSucceed) CONNECT_WITH_ADDR_AND_PORTu(ui,sock,htonl(INADDR_LOOPBACK),htons(portInHostByteOrder),caseInvalidSocket,caseConnectErr,caseSucceed)
#define CONNECT_INADDR_ANY_WITH_PORT_IN_HOST_BYTE_ORDERu(ui,sock,portInHostByteOrder,caseInvalidSocket,caseConnectErr,caseSucceed) CONNECT_WITH_ADDR_AND_PORTu(ui,sock,htonl(INADDR_ANY),htons(portInHostByteOrder),caseInvalidSocket,caseConnectErr,caseSucceed)//normally you don't use this

//note MSG_WAITALL as the last param for recv() might be useful in come cases (recv has better performance maybe), but note that you may want to avoid it to be safe (who knows if it nullifies timeout/nonblocking on any platforms?)

//note select may be not enough, you probably should always use O_NONBLOCK socket (MSG_DONTWAIT may be useful be it's not available on some OSs, e.g. Windows)
/*//source http://man7.org/linux/man-pages/man2/select.2.html
Under Linux, select() may report a socket file descriptor as "ready
for reading", while nevertheless a subsequent read blocks.  This
could for example happen when data has arrived but upon examination
has wrong checksum and is discarded.  There may be other
circumstances in which a file descriptor is spuriously reported as
ready.  Thus it may be safer to use O_NONBLOCK on sockets that should
not block.
*/
/*//source https://en.wikipedia.org/wiki/Berkeley_sockets
When using blocking sockets, special consideration should be given to accept() as it may still block after indicating readability if a client disconnects during the connection phase.*/

//learn from sdl net
//note this macro should equal true when error occurs, otherwise false
#if defined(__BEOS__) && defined(SO_NONBLOCK)
	/* On BeOS r5 there is O_NONBLOCK but it's for files only */
	//? the 4th param is long in sdl, but apache apr uses int?
#define SET_NONBLOCK_MODE_FOR_SOCKETu(ui,sock,caseErr,caseDone) \
	{int ui##tmp=1; if(0>setsockopt(sock, SOL_SOCKET, SO_NONBLOCK, &ui##tmp, sizeof(int))){caseErr}else{caseDone}}
#elif defined(O_NONBLOCK)
#define SET_NONBLOCK_MODE_FOR_SOCKETu(ui,sock,caseErr,caseDone) \
	{if(-1==fcntl(sock, F_SETFL, O_NONBLOCK)){caseErr}else{caseDone}}
#elif defined(_WIN32)
#define SET_NONBLOCK_MODE_FOR_SOCKETu(ui,sock,caseErr,caseDone) \
	{u_long ui##tmp=1; if(ioctlsocket(sock, FIONBIO, &ui##tmp)){caseErr}else{caseDone}}
#elif defined(__OS2__)
#define SET_NONBLOCK_MODE_FOR_SOCKETu(ui,sock,caseErr,caseDone) \
	{int ui##tmp=1; if(-1==ioctl(sock, FIONBIO, &ui##tmp)){caseErr}else{caseDone}}
#else
#error cannot set non-blocking mode?
#endif

//note this macro should equal true when the reason of error is "nonblocking socket would block". Otherwise false.
//note this macro is for accept(), recv() (and any other function?)
//? sdl never checks this. this macro should work on Windows and Posix, but what about other OSs?
//you may not really need this macro, e.g. you use select to find a readable socket. Yet it would block. This may be due to something wrong in the underlying layer (checksum, disconnect, etc.). In such cases you probably want to close the socket right away anyway. Thus no need to check the reason.
#ifdef _WIN32
#define CHECK_IF_REASON_OF_ERROR_IS_EAGAIN_EWOULDBLOCK (WSAGetLastError()==WSAEWOULDBLOCK)
#else
#define CHECK_IF_REASON_OF_ERROR_IS_EAGAIN_EWOULDBLOCK (errno==EAGAIN||errno==EWOULDBLOCK)//?could reading errno be expensive?
#endif

//note winsock2 recv is int(SOCKET,char *,int,int). Yet posix is ssize_t(int,void *,size_t,int)
#define RECV_WITHOUT_FLAGS(sock,pbuf,len) \
/*errno=0; OR WSASetLastError(0); //?is this really necessary? */\
recv(sock,pbuf,len,0)//undone casting to size_t/int necessary?

//note winsock2 send is int(SOCKET,const char *,int,int). Yet posix is ssize_t(int,const void *,size_t,int)
#define SEND_WITHOUT_FLAGS(sock,pbuf,len) \
/*errno=0; OR WSASetLastError(0); //?is this really necessary? */\
send(sock,pbuf,len,0)

//badly written code. never use ADDRESS_OF_COPY
//#define setsocketopt for SO_RCVTIMEO and SO_SNDTIMEO (sdl doesn't use these 2). You may use them but note they are drastically different on Windows and other OSs
//note these 4 sets of macros evaluate to ZERO if succeeds (SOCKET_ERROR / -1 is returned if error occurs on Windows / posix)
//note on both windows and posix, 4th arg of setsockopt is of type `const char *`
//note winsock2 recv times out with WSAETIMEDOUT (what about send?), yet posix sets errno to EAGAIN/EWOULDBLOCK for both recv and send
//note winsock2: "If a send or receive operation times out on a socket, the socket state is indeterminate, and should not be use".
//so setting timeout can be USELESS in most cases
#ifdef _WIN32
#define SETSOCKOPT_SO_RCVTIMEO_MILLISECOND(sock,msec) setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)ADDRESS_OF_COPY(DWORD,msec),sizeof(DWORD))
#else
#define SETSOCKOPT_SO_RCVTIMEO_MILLISECOND(sock,msec) setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)ADDRESS_OF_COPY(struct timeval,{.tv_sec=(msec)/1000, .tv_usec=(msec)%1000}),sizeof(struct timeval))
#endif
#ifdef _WIN32
#define SETSOCKOPT_SO_RCVTIMEO_PTIMEVAL(sock,ptv) setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)ADDRESS_OF_COPY( DWORD, (DWORD)(ptv)->tv_sec*1000+(DWORD)(ptv)->tv_usec ),sizeof(DWORD))
#else
#define SETSOCKOPT_SO_RCVTIMEO_PTIMEVAL(sock,ptv) setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char *)(ptv),sizeof(struct timeval))
#endif
#ifdef _WIN32
#define SETSOCKOPT_SO_SNDTIMEO_MILLISECOND(sock,msec) setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)ADDRESS_OF_COPY(DWORD,msec),sizeof(DWORD))
#else
#define SETSOCKOPT_SO_SNDTIMEO_MILLISECOND(sock,msec) setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)ADDRESS_OF_COPY(struct timeval,{.tv_sec=(msec)/1000, .tv_usec=(msec)%1000}),sizeof(struct timeval))
#endif
#ifdef _WIN32
#define SETSOCKOPT_SO_SNDTIMEO_PTIMEVAL(sock,ptv) setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)ADDRESS_OF_COPY( DWORD, (DWORD)(ptv)->tv_sec*1000+(DWORD)(ptv)->tv_usec ),sizeof(DWORD))
#else
#define SETSOCKOPT_SO_SNDTIMEO_PTIMEVAL(sock,ptv) setsockopt(sock,SOL_SOCKET,SO_SNDTIMEO,(char *)(ptv),sizeof(struct timeval))
#endif




#define PORTABLE_RT_SO_TIMEVAL_VERYLONG_TV_SEC 4294967//the biggest tv_sec libsdl uses, should be a safe cross-platform tv_sec

//note timeval obj may be modifed by select() on some platforms (windows doesn't modify it?)
//this function can be useful since select() with the same timeval value over and over is common
#define SELECT_READABILITY_WITH_COPIED_TIMEVALs(ret,nfds,pfdset,tv) {struct timeval UI_IN_LIMITED_SCOPE=tv; ret=select(nfds,pfdset,NULL,NULL,&UI_IN_LIMITED_SCOPE);}
#define SELECT_WRITABILITY_WITH_COPIED_TIMEVALs(ret,nfds,pfdset,tv) {struct timeval UI_IN_LIMITED_SCOPE=tv; ret=select(nfds,NULL,pfdset,NULL,&UI_IN_LIMITED_SCOPE);}

#ifdef _WIN32
#define SELECT_READABLE_FDS_WITH_COPIED_TIMEVALs(ret,nfds,pfdset,tv) SELECT_READABILITY_WITH_COPIED_TIMEVALs(ret,0,pfdset,tv)
#define SELECT_SINGLE_READABLE_FD_WITH_COPIED_TIMEVALs(ret,fd,pfdset,tv) SELECT_READABLE_FDS_WITH_COPIED_TIMEVALs(ret,,pfdset,tv)
#define SELECT_SINGLE_WRITABLE_FD_WITH_COPIED_TIMEVALs(ret,fd,pfdset,tv) SELECT_WRITABILITY_WITH_COPIED_TIMEVALs(ret,0,pfdset,tv)
#else
#define SELECT_READABLE_FDS_WITH_COPIED_TIMEVALs(ret,nfds,pfdset,tv) SELECT_READABILITY_WITH_COPIED_TIMEVALs(ret,nfds,pfdset,tv)
#define SELECT_SINGLE_READABLE_FD_WITH_COPIED_TIMEVALs(ret,fd,pfdset,tv) SELECT_READABLE_FDS_WITH_COPIED_TIMEVALs(ret,((fd)+1),pfdset,tv)
#define SELECT_SINGLE_WRITABLE_FD_WITH_COPIED_TIMEVALs(ret,fd,pfdset,tv) SELECT_WRITABILITY_WITH_COPIED_TIMEVALs(ret,((fd)+1),pfdset,tv)
#endif

//note different from sdl, retval_int>0 is not checked here
#define CHECK_SELECT_RESULTs(retval_int,caseErr,caseTimeout,caseElse) {if(retval_int==SOCKET_ERROR){caseErr}else if(!retval_int){caseTimeout}else{caseElse}}

#define SELECT_SINGLE_READABLE_FD_WITH_COPIED_TIMEVAL_N_CHECKu(ui,fd,pfdset,tv,caseErr,caseTimeout,caseElse){\
	FD_ZERO(pfdset); FD_SET(fd,pfdset);\
	int ui##selectRet; SELECT_SINGLE_READABLE_FD_WITH_COPIED_TIMEVALs(ui##selectRet,fd,pfdset,tv)\
	CHECK_SELECT_RESULTs(ui##selectRet,caseErr,caseTimeout,caseElse)\
}
#define SELECT_SINGLE_WRITABLE_FD_WITH_COPIED_TIMEVAL_N_CHECKu(ui,fd,pfdset,tv,caseErr,caseTimeout,caseElse){\
	FD_ZERO(pfdset); FD_SET(fd,pfdset);\
	int ui##selectRet; SELECT_SINGLE_WRITABLE_FD_WITH_COPIED_TIMEVALs(ui##selectRet,fd,pfdset,tv)\
	CHECK_SELECT_RESULTs(ui##selectRet,caseErr,caseTimeout,caseElse)\
}

//learn from nspr and stackoverflow, nonblocking socket can get ewouldblock. checking 0 returned by send should be unnecessary for most platforms (even if 0 return value is possible, it will become error after a few tries?).
#define SELECT_N_SEND_ALL_WITH_OFF_N_COPIED_TIMEVALu(ui,offset,buf,si,fd,pfdset,tv,caseTimeout,caseErr,caseDone){\
	size_t ui##off=(size_t)(offset);\
	ui##selectLbl:\
	SELECT_SINGLE_WRITABLE_FD_WITH_COPIED_TIMEVAL_N_CHECKu(ui##select_,fd,pfdset,tv,caseErr,caseTimeout,{\
		int_for_int_ssize_t ui##sendRet = SEND_WITHOUT_FLAGS(fd,(buf)+ui##off,(si)-ui##off);\
		if(ui##sendRet==SOCKET_ERROR){caseErr}\
		else{ui##off+=ui##sendRet; if(ui##off!=si){assert(ui##off<si);goto ui##selectLbl;} {caseDone}}\
	})\
}
#define SELECT_N_SEND_ALL_WITH_COPIED_TIMEVALu(ui,buf,si,fd,pfdset,tv,caseTimeout,caseErr,caseDone) SELECT_N_SEND_ALL_WITH_OFF_N_COPIED_TIMEVALu(ui,0,buf,si,fd,pfdset,tv,caseTimeout,caseErr,caseDone)

#define SELECT_N_RECV_AT_LEAST_SI_WITH_COPIED_TIMEVALu(ui,numOfReceivedByt,recvBuf,maxSi,leastSi,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone){\
	numOfReceivedByt=0;\
	SELECT_N_RECV_AT_LEAST_SI_WITH_OFF_N_COPIED_TIMEVALu(ui,numOfReceivedByt,recvBuf,maxSi,leastSi,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone)\
}
//recv at least (leastSi - offset) bytes
#define SELECT_N_RECV_AT_LEAST_SI_WITH_OFF_N_COPIED_TIMEVALu(ui,offset,recvBuf,maxSi,leastSi,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone){\
	ui##select:\
	SELECT_N_RECV_WITH_OFF_N_COPIED_TIMEVALu(ui##iteration_,offset,recvBuf,maxSi,fd,pfdset,tv,(offset)+=,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,{\
		if((offset)<(leastSi)) goto ui##select; {caseDone}\
	})\
}
//note you may be tempted to used goto ui##start_cmp_offset_n_leastSi, but you should not use it, 1stly spaghetti goto probably cannot be optimized by compiler. 2ndly c++ doesn't allow skip declaration?
#define ENSURE_AT_LEAST_SI_WITH_SELECT_N_RECV_WITH_OFF_N_COPIED_TIMEVALu(ui,offset,recvBuf,maxSi,leastSi,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone){\
	if((offset)<(leastSi)){\
		SELECT_N_RECV_AT_LEAST_SI_WITH_OFF_N_COPIED_TIMEVALu(ui##sel_,offset,recvBuf,maxSi,leastSi,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone)\
	}else{caseDone}\
}
#define SELECT_N_RECV_WITH_OFF_N_TMP_N_COPIED_TIMEVALu(ui,offset,recvBuf,maxSi,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone) SELECT_N_RECV_WITH_OFF_N_COPIED_TIMEVALu(ui,offset,recvBuf,maxSi,fd,pfdset,tv,EXPAND_TO_NOTHING,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone)
#define SELECT_N_RECV_WITH_OFF_N_COPIED_TIMEVALu(ui,offset,recvBuf,maxSi,fd,pfdset,tv,NOT_NECESSARILY_A_FUNC_LIKE_MACRO_TAKES_SIZE,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone){\
	int_for_int_ssize_t ui##tmp;\
	SELECT_N_RECV_WITH_OFF_N_BUF_N_COPIED_TIMEVALu(ui##sel_,offset,recvBuf,maxSi,fd,pfdset,tv,ui##tmp,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,{\
		NOT_NECESSARILY_A_FUNC_LIKE_MACRO_TAKES_SIZE(ui##tmp); {caseDone}\
	})\
}
#define SELECT_N_RECV_WITH_OFF_N_BUF_N_COPIED_TIMEVALu(ui,offset,recvBuf,maxSi,fd,pfdset,tv,buf_for_int_ssize_t,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseDone){\
	SELECT_SINGLE_READABLE_FD_WITH_COPIED_TIMEVAL_N_CHECKu(ui##select_,fd,pfdset,tv,caseSelectErr,caseTimeout,{\
		(buf_for_int_ssize_t) = RECV_WITHOUT_FLAGS(fd,(recvBuf)+offset,(maxSi)-offset);\
		if((buf_for_int_ssize_t)==SOCKET_ERROR){caseRecvErr}\
		else if(!(buf_for_int_ssize_t)){casedClosed}\
		else{caseDone}\
	})\
}

#define SELECT_N_RECV_TO_FIND_CHR_WITH_COPIED_TIMEVALu(ui,pChr,dtlen,recvBuf,maxSi,chr,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseFound){\
	ui##select:\
	SELECT_N_RECV_WITH_OFF_N_TMP_N_COPIED_TIMEVALu(ui##select_,0,recvBuf,maxSi,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,{\
		pChr=memchr(recvBuf,chr,ui##select_tmp); if(NULL==(pChr)) goto ui##select; dtlen=ui##select_tmp; {caseFound}\
	})\
}
//strLen must be 2 or greater, or it's undefined behavior? (bc (str)+1 is an invalid ptr?)
//maxSi must be -ge to strLen
//note you should try to use less memmove / memcpy (note each time you might copy HUGE size)
//note you should try to use less select / recv / send, bc they are syscalls
#define SELECT_N_RECV_TO_FIND_STR_WITH_COPIED_TIMEVALu(ui,pStr,dtlen,recvBuf,maxSi,str,strLen,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseFound){\
	ui##st:\
	SELECT_N_RECV_TO_FIND_CHR_WITH_COPIED_TIMEVALu(ui##findchr_,pStr,dtlen,recvBuf,maxSi,(str[0]),fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,{\
		ui##chk:\
		if((recvBuf)+(dtlen)-(pStr)<(strLen)){\
			if((recvBuf)+(maxSi)-(pStr)<(strLen)) {MEMMOVE_WITHIN_OBJ_TO_DISCARD_HEADs(recvBuf,pStr,dtlen) pStr=recvBuf;}\
			SELECT_N_RECV_AT_LEAST_SI_WITH_OFF_N_COPIED_TIMEVALu(ui##sel_,dtlen,recvBuf,maxSi,( (pStr)-(recvBuf)+strLen ),fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,{goto ui##cmp;})\
		}else{\
			ui##cmp:\
			if(memcmp((str)+1,(pStr)+1,(strLen)-1)){\
				pStr=memchr((pStr)+1,str[0],(recvBuf)+(dtlen)-(pStr)-1);\
				if(NULL==pStr){goto ui##st;}\
				else{goto chk;}\
			}else{caseDone}\
		}\
	})\
}
#define FIND_STR_WITH_FIRST_CHR_FOUND_N_SELECT_N_RECV_WITH_COPIED_TIMEVALu(){\
	if((recvbuf)+(dtlen)-(pstr)<(strlen)){\
		if((recvbuf)+(maxsi)-(pstr)<(strlen)){MEMMOVE_WITHIN_OBJ_TO_DISCARD_HEADs(recvbuf,pstr,dtlen) pStr=recvBuf;}\
	}\
}//undone what if you have an offset and want to do a memmem first? need to rewrite?
	
#define FIND_CHR_WITH_OFF_N_SELECT_N_RECV_WITH_COPIED_TIMEVALu(){\
	pChr=memchr(recvBuf,chr,)\
}//undone remove?
#define FIND_ONE_CHR_PAST_STREND_WITH_FIRST_CHR_FOUND_N_SELECT_N_RECV_WITH_COPIED_TIMEVALu(){\
	MEMCMP
	\
}
#define FIND_ONE_CHR_PAST_STREND_WITH_SELECT_N_RECV_WITH_COPIED_TIMEVALu(){\
	\
}

//undone bad implementation? remove?
#define SELECT_N_RECV_N_FIND_STR_WITH_COPIED_TIMEVALs(pStr,dtlen,buf,sizeofBuf,str,strLen,fd,pfdset,tv,lblSelectErr,lblTimeout,lblClosed,lblRecvErr,caseFound){\
	SELECT_N_RECV_AT_LEAST_SI_WITH_COPIED_TIMEVALu(xxx_,dtlen,buf,sizeofBuf,strLen,fd,pfdset,tv,lblSelectErr,lblTimeout,goto lblClosed;,goto lblRecvErr;,)\
	pStr=(buf);\
	SELECT_N_RECV_TO_FIND_STR_WITH_COPIED_TIMEVALu(pStr,dtlen,buf,sizeofBuf,str,strLen,fd,pfdset,tv,lblSelectErr,lblTimeout,lblClosed,lblRecvErr,caseFound)\
}
//undone bad?

#define SELECT_N_RECV_WITH_COPIED_TIMEVAL_TO_FIND_STRu(ui,pStr,dtlen,buf,sizeofBuf,str,strLen,fd,pfdset,tv,){\
	find chr first\
	SELECT_N_RECV_WITH_OFF_N_COPIED_TIMEVALu(ui##try_,0,buf,sizeofBuf,fd,pfdset,tv,(dtlen)=,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,{\
		\
	})\
}

//strLen must be 2 or greater
//in exceptional cases, the value of dtlen is still correct length of data
#define SELECT_N_RECV_TO_FIND_STR_WITH_COPIED_TIMEVALu(ui,pStr,dtlen,buf,sizeofBuf,str,strLen,fd,pfdset,tv,lblSelectErr,lblTimeout,lblClosed,lblRecvErr,caseFound){\
	for(;;){\
		pStr = memchr(pStr,(str)[0],(buf)+(dtlen)-pStr);\
		if(NULL==pStr){\
			SELECT_N_RECV_AT_LEAST_SI_WITH_COPIED_TIMEVALu(ui##nochar_,dtlen,buf,sizeofBuf,strLen,fd,pfdset,tv,lblSelectErr,lblTimeout,goto lblClosed;,goto lblRecvErr;,)\
			pStr=(buf); continue;\
		}else{\
			if((uintmax_t)((buf)+(dtlen)-pStr)<(uintmax_t)(strLen)){\
				dtlen=(buf)+(dtlen)-pStr;\
				memmove(buf,pStr,dtlen);\
				SELECT_N_RECV_AT_LEAST_SI_WITH_OFF_N_COPIED_TIMEVALu(ui##morechars_,dtlen,buf,sizeofBuf,strLen,fd,pfdset,tv,lblSelectErr,lblTimeout,goto lblClosed;,goto lblRecvErr;,)\
				pStr=(buf);\
			}\
			if(memcmp(pStr+1,str+1,(strLen)-1)){\
				++pStr; continue;\
			}\
			{caseFound}break;\
		}\
	}\
}
#define SELECT_N_RECV_TO_FIND_CHR_WITH_COPIED_TIMEVALs(pChr,dtlen,recvBuf,maxSi,chr,fd,pfdset,tv,lblSelectErr,lblTimeout,caseClosed,caseRecvErr,caseFound){\
	pChr = memchr(pChr,(chr),(recvBuf)+(dtlen)-pChr);\
	if(NULL==pChr){\
		SELECT_N_RECV_N_FIND_CHR_WITH_COPIED_TIMEVALs(pChr,dtlen,recvBuf,maxSi,chr,fd,pfdset,tv,lblSelectErr,lblTimeout,caseClosed,caseRecvErr,caseFound)\
	}else{caseFound}\
}
#define SELECT_N_RECV_AT_LEAST_UNTIL_CHR_WITH_OFF_N_COPIED_TIMEVALu(ui,pChr,chr,offset,recvBuf,maxSi,fd,pfdset,tv,caseSelectErr,caseTimeout,caseClosed,caseRecvErr,caseUnfound,caseFound){\
	ui##select:\
	SELECT_SINGLE_READABLE_FD_WITH_COPIED_TIMEVAL_N_CHECKu(ui##select_,fd,pfdset,tv,caseSelectErr,caseTimeout,{\
		int_for_int_ssize_t ui##recvRet = RECV_WITHOUT_FLAGS(fd,(recvBuf)+offset,(maxSi)-offset);\
		if(ui##recvRet==SOCKET_ERROR){caseRecvErr}\
		else if(!ui##recvRet){casedClosed}\
		else{\
			pChr=memchr((recvBuf)+offset,(chr),ui##recvRet); offset+=ui##recvRet;\
			if(NULL==pChr){\
				if((offset)==(maxSi)){caseUnfound}\
				else{goto ui##select;}\
			}else{caseFound}\
		}\
	})\
}
#define MEMCHR_OR_SELECT_N_RECV_AT_LEAST_UNTIL_CHR_WITH_OFF_N_COPIED_TIMEVALs(pChr,chr,offset,recvBuf,maxSi,fd,pfdset,tv,lblSelectErr,lblTimeout,caseClosed,caseRecvErr,caseUnfound,caseFound){\
	pChr=memchr((recvBuf),(chr),offset);\
	if(NULL==pChr){\
		SELECT_N_RECV_AT_LEAST_UNTIL_CHR_WITH_OFF_N_COPIED_TIMEVALs(pChr,chr,offset,recvBuf,maxSi,fd,pfdset,tv,lblSelectErr,lblTimeout,caseClosed,caseRecvErr,caseUnfound,caseFound)\
	}\
}

//note winsock2 already has `typedef int socklen_t`;, if you use winsock1 then you might need typedef int socklen_t
//note winsock2 3rd arg is of type int *, yet posix 3rd arg is of type socklen_t *
#define ACCEPT_WITH_CLIADDRs(retbuf,sock,cliaddr){socklen_t tmp_ACCEPT_WITH_CLIADDRs=sizeof(cliaddr); retbuf=accept(sock,(struct sockaddr *)&(cliaddr),&tmp_ACCEPT_WITH_CLIADDRs);}
#define ACCEPT_WITH_CLIADDR_N_SET_NONBLOCK_MODEu(ui,sockbuf,listensock,cliaddr,caseErr,caseDone){\
	ACCEPT_WITH_CLIADDRs(sockbuf,listensock,cliaddr)\
	if(INVALID_SOCKET==sockbuf){caseErr}else{\
		SET_NONBLOCK_MODE_FOR_SOCKETu(ui##set_,sockbuf,{BERKELEY_SO_CLOSE(sockbuf);{caseErr}},{caseDone})\
	}\
}

#ifdef _WIN32
#define CHECK_FD_SELECTABLE(sock) 1//FD_SETSIZE means NUM of fds on windows, so you need to count how many fds are added via FD_SET. (boost asio doesn't use FD_SETSIZE in winsock, it uses it's own win_fd_set instead, which is not fixed-size like FD_SETSIZE, but a dynamically increasing FD set!)
#define CASE_FD_UNSELECTABLE(sock,caseUnsel)
#else
#define CHECK_FD_SELECTABLE(sock) ( (sock)<FD_SETSIZE )
#define CASE_FD_UNSELECTABLE(sock,caseUnsel) {if(! (CHECK_FD_SELECTABLE(sock)) ){caseUnsel}}
#endif

//no need to define ACCEPT,SEND,RECV, just use berkeley accept(,,),send(,,,),recv(,,,)
//no need to define fd_set,FD_SET,FD_ZERO,FD_ISSET,struct timeval, just use them as convention
//what about FD_CLR? is it cross-platform too? unsure?
//FD_SETSIZE has different meanings for Windows and POSIX. On windows, it means NUM of fds. Yet on posix it means a VALUE that must be greater than fds


#ifdef _WIN32
//#define PORTABLE_RT_SO_FD_SET_T fd_set *//you may #define such a type if necessary: on windows it's just fd_set *, on posix it has an extra nfds as the highest fd+1
#define PORTABLE_RT_SO_FD_AS_NFDSd(nfds)
#define PORTABLE_RT_SO_SET_NFDSs(nfds,value)
#define FD_SET_N_UPDATE_NFDS_IF_NEEDEDs(nfds,fd,pfdset){\
	FD_SET(fd,pfdset);\
}
#define CHK_TOO_MANY_FDS_FOR_FDSETs(numfds,caseToomanyfds,caseOk) if(numfds>FD_SETSIZE){caseToomanyfds}else{caseOk}
//note on windows there is fd_count in the struct: typedef struct fd_set {u_int  fd_count; SOCKET fd_array[FD_SETSIZE];} fd_set;
//remove, not well written
#define FD_SET_N_UPDATE_NFDS_N_CHK_COUNT_IF_NEEDEDs(nfds,fd,pfdset,caseToomanyfds) {\
	if( (pfdset)->fd_count>=FD_SETSIZE ){caseToomanyfds}\
	else{FD_SET(fd,pfdset);}\
}
#define FD_SET_IF_SELECTABLE_N_UPDATE_NFDS_N_CHK_COUNT_IF_NEEDEDs(caseUnselectable,nfds,fd,pfdset,caseToomanyfds) FD_SET_N_UPDATE_NFDS_N_CHK_COUNT_IF_NEEDEDs(nfds,fd,pfdset,caseToomanyfds)
#else
#define PORTABLE_RT_SO_FD_AS_NFDSd(nfds) portable_rt_socket_t nfds;
#define PORTABLE_RT_SO_SET_NFDSs(nfds,value) {nfds=(value);}
#define FD_SET_N_UPDATE_NFDS_IF_NEEDEDs(nfds,fd,pfdset){\
	if((fd)>=(nfds)){ nfds=(fd)+1; } FD_SET(fd,pfdset);\
}
#define CHK_TOO_MANY_FDS_FOR_FDSETs(numfds,caseToomanyfds,caseOk)
//remove, not well written
#define FD_SET_N_UPDATE_NFDS_N_CHK_COUNT_IF_NEEDEDs(nfds,fd,pfdset,caseToomanyfds) FD_SET_N_UPDATE_NFDS_IF_NEEDEDs(nfds,fd,pfdset)
//you might not need this macro bc you usually already checked selectability after socket()/accept()
//note if nfds is greater than FD_SETSIZE (i.e. not a real nfds), this macro results in undefined behavior
#define FD_SET_IF_SELECTABLE_N_UPDATE_NFDS_N_CHK_COUNT_IF_NEEDEDs(caseUnselectable,nfds,fd,pfdset,caseToomanyfds) {\
	if((fd)>=(nfds)){ if(CHECK_FD_SELECTABLE(fd)){nfds=(fd)+1;FD_SET(fd,pfdset);}else{caseUnselectable} }\
	else{ FD_SET(fd,pfdset); }\
}
#endif
//no #define for FD_CLR bc you probably won't use it anyway
#define SET_FDSET_N_NFDS_N_SELECT_READABLE_FDS_WITH_COPIED_TIMEVAL_N_SELECTABLE_FDSu(ui,pfds,numfds,tv,caseErr,caseTimeout,caseElse){\
	CHK_TOO_MANY_FDS_FOR_FDSETs(numfds,{caseToomanyfds},{\
		PORTABLE_RT_SO_FD_AS_NFDSd(ui##nfds)\
		PORTABLE_RT_SO_SET_NFDSs(ui##nfds,*(pfds)+1)\
		fd_set ui##mask; FD_ZERO(ui##mask);\
		for(size_t i=0;i<(numfds);i++){\
			FD_SET_N_UPDATE_NFDS_IF_NEEDEDs(ui##nfds,(pfds)[i],&ui##mask)\
		}\
		int ui##selectRet;\
		SELECT_READABLE_FDS_WITH_COPIED_TIMEVALs(selret,ui##nfds,&ui##mask,tv)\
		CHECK_SELECT_RESULTs(ui##selectRet,caseErr,caseTimeout,caseElse)\
		//undone you need a bit array here to store all detected sockets!
		\
	})\
}


//undone udp hole punching?

