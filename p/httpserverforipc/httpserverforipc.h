#pragma once

//undone no ssl, just listen, accept, recv, send

//support keep-alive?

//#include "portable_rt_so.h"
#include "portable_rt_tcp.h"

//undone try to use 28 for testing purpose (may find some bug when you use a smaller buffer)
//undone also try 65536 for testing
static char receiveArrBuf[256];//big enough for storing "\r\ncontent-length: " and storing content-length digits

//static void recvHttpRequest(void){
#define RECV_HTTP_REQUEST_N_RETAIN_BODY_WITH_COPIED_TIMEVALu(ui,body,receivedsi,fd,pfds,tv,caseClosedOrFail,caseTimeout,caseDone){\
	/*ptrdiff_t*/size_t ui##recvoff;\
	char *ui##pCr;\
	SELECT_N_RECV_N_FIND_STR_WITH_COPIED_TIMEVALs(ui##pCr,ui##recvoff,receiveArrBuf,(sizeof receiveArrBuf-1),"\r\n",(sizeof"\r\n"-1),fd,pfds,tv,lblSelectErr,lblTimeout,lblClosed,lblRecvErr,)\
	for(;;){\
		ui##recvoff=receiveArrBuf+ui##recvoff-ui##pCr-(sizeof "\r\n"-1);\
		memmove(receiveArrBuf,ui##pCr+(sizeof"\r\n"-1),ui##recvoff);\
		if(ui##recvoff<sizeof"Content-Length: "){\
			SELECT_N_RECV_AT_LEAST_SI_WITH_OFF_N_COPIED_TIMEVALu(ui##recHeader_,ui##recvoff,receiveArrBuf,(sizeof receiveArrBuf-1),(sizeof"Content-Length: "),fd,pfds,tv,lblSelectErr,lblTimeout,{goto lblClosedOrFail;},{goto lblClosedOrFail;},)\
		}\
		if(!memcmp(receiveArrBuf,"\r\n",(sizeof"\r\n"-1))){}/*//undone cannot find content len in header, abort connection*/\
		if(memcmp(receiveArrBuf,"Content-Length: ",(sizeof"Content-Length: "-1))){\
			ui##pCr=receiveArrBuf+1;\
			SELECT_N_RECV_TO_FIND_STR_WITH_COPIED_TIMEVALs(ui##pCr,ui##recvoff,receiveArrBuf,(sizeof receiveArrBuf-1),"\r\n",(sizeof"\r\n"-1),fd,pfds,tv,lblSelectErr,lblTimeout,lblClosed,lblRecvErr,)\
			continue;\
		}\
		ui##recvoff-=(sizeof"Content-Length"-1);\
		memmove(receiveArrBuf,receiveArrBuf+(sizeof"Content-Length: "-1),ui##recvoff);\
		MEMCHR_OR_SELECT_N_RECV_AT_LEAST_UNTIL_CHR_WITH_OFF_N_COPIED_TIMEVALs(ui##pCr,'\r',ui##recvoff,receiveArrBuf,(sizeof receiveArrBuf-1),fd,pfds,tv,lblSelectErr,lblTimeout,caseClosed,caseRecvErr,caseUnfound,)\
		unsigned long long contentLen;\
		STRTOULL_WITHOUT_ENDPTRs(contentLen,receiveArrBuf,10,{\
			puts("parse content len failed"); return RETVALFAILURE;/*//undone*/\
		},)\
		if(contentLen>(size_t)-1) { LOGLITERALs("request is too big");/*//undone ?*/\
		}\
		SELECT_N_RECV_TO_FIND_STR_WITH_COPIED_TIMEVALs(ui##pCr,ui##recvoff,receiveArrBuf,(sizeof receiveArrBuf-1),"\r\n\r\n",(sizeof"\r\n\r\n"-1),fd,pfds,tv,lblSelectErr,lblTimeout,lblClosed,lblRecvErr,)\
		char* reqBody = malloc(contentLen+10);/*//undone change to contentLen+1*/\
		if(NULL==reqBody) return RETVALFAILURE;/*//undone*/\
		ui##recvoff = receiveArrBuf+ui##recvoff-(ui##pCr+(sizeof"\r\n\r\n"-1));\
		memcpy(reqBody,ui##pCr+(sizeof"\r\n\r\n"-1),ui##recvoff);\
		if(ui##recvoff<contentLen){/*//undone cast from signed to unsigned?*/\
			SELECT_N_RECV_AT_LEAST_SI_WITH_OFF_N_COPIED_TIMEVALu(ui##recRest_,ui##recvoff,reqBody,(contentLen+9),contentLen,fd,pfds,tv,lblSelectErr,lblTimeout,caseClosed,caseRecvErr,)\
		}\
		receivedsi = ui##recvoff;/*//there are trailing \r\n not received?*/\
\
		/*//return ui##pCr+(sizeof"\r\n\r\n"-1)*/\
\
	}\
	while(0){fail_close: BERKELEY_SO_CLOSE(fd);}\
	while(0){lblClosedOrFail: BERKELEY_SO_CLOSE(fd); {caseClosedOrFail}}\
	while(0){lblTimeout: {caseTimeout}}\
}

	/*//starting from the 2nd \r\n after headers (included or not?), counting recv until content-length bytes are collected.*/
//}

//note you try to manifest yourself as a primitive web server, so chunked encoding is not used, and the less headers, the better
//"Aside from the cases defined above, in the absence of Transfer-Encoding, an origin server SHOULD send a Content-Length header field when the payload body size is known prior to sending the complete header section." So you have to use Content-Length?
//"If a Content-Type header field is not present, the recipient MAY either assume a media type of application/octet-stream (RFC2046, Section 4.5.1) or examine the data to determine its type." So you should include content-type to prevent any possible discrepancies between browsers?
//? content-type is "text/plain; charset=utf-8" OR "application/octet-stream" OR "text/plain; charset=x-user-defined"
//Server header is omitted, possible any browser requires it?
//Date is omitted, which is necessary for browser to caching response. (HTTP-date is painful to construct with time.h functions)
//For HTTP 1.1, "persistent connections are the default behavior of any HTTP connection", so Connection: keep-alive is unnecessary
//"Access-Control-Allow-Origin: *" maybe necessary?
#define SEND_HTTP_RESPONSE_HEADERS_WITH_CL_INTEGER_N_COPIED_TIMEVALu(ui,fd,pfds,tv,contentLength,caseTimeout,caseFail,caseDone){\
	char ui##temarr[21+3];/*enough for the usual UINT64_MAX 18446744073709551615, and extra 4 bytes for double CRLF*/\
	int ui##intbuf=snprintf(ui##temarr,21,"%"PRIuMAX,(uintmax_t)(contentLength));\
	if(ui##intbuf<0||ui##intbuf>=21){caseFail}else{\
		SELECT_N_SEND_ALL_WITH_COPIED_TIMEVALu(ui##sendLit_,LITERAL_COMMA_LEN("HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: "),fd,pfds,tv,caseTimeout,caseFail,{\
			memcpy(ui##temarr+ui##intbuf,"\r\n\r\n",4);\
			SELECT_N_SEND_ALL_WITH_COPIED_TIMEVALu(ui##sendCL_,ui##temarr,(ui##intbuf+4),fd,pfds,tv,caseTimeout,caseFail,caseDone)\
		})\
	}\
}//undone "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: XXXX\r\n"

//static void acceptAndCheckHttp(struct timeval to){
//
//}





//#include "sdlinter.h"
//#include "SDL_net.h"


#define HTTPSERVERFORIPC_STREXIT "\0"//undone is this okay?
#define HTTPSERVERFORIPC_STRCONFIRMEXIT "~"//a magic code for confirmation


////not thread safe
//static void init_httpserverforipc(void){
//    if(numofmodules_using_sdlnet){
//        ++numofmodules_using_sdlnet;
//    }else{
//        if(numofmodules_using_sdl){
//            ++numofmodules_using_sdl;
//        }else{
//            CALL_SDL_Init_N_SETINT_IF_SUCCESSs(0,{
//                //undone log error through SDL_GetError();
//                //printf("SDL_Init: %s\n",SDL_GetError());
//            },);
//        }
//        CALL_SDLNet_Init_N_SETINT_IF_SUCCESSs({
//            //undone log error through SDL_GetError();
//            //printf("SDLNet_Init: %s\n",SDLNet_GetError());
//        },)
//    }
//}
//
////not thread safe
//static void quit_httpserverforipc(void){
//    --numofmodules_using_sdlnet;
//    if(!numofmodules_using_sdlnet) SDLNet_Quit();
//    --numofmodules_using_sdl;
//    if(!numofmodules_using_sdl) SDL_Quit();
//}
//
//static void startListening(Uint16 port){
//    //sdl doesn't support binding to an ip (including 127.0.0.1)
//    IPaddress ip;
//    if(SDLNet_ResolveHost(&ip,NULL,port)==-1){
//        //undone printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
//    }
//    TCPsocket server=SDLNet_TCP_Open(&ip);
//    if(!server)
//	{
//		//printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
//	}
//
//}
//
//static void acceptConn(void *allowedIpInBE){
//	client=SDLNet_TCP_Accept(server);
//	if(!client)
//	{ /* no connection accepted */
//		/*//undone printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError()); */
//	}
//	
//	/* get the clients IP and port number */
//	remoteip=SDLNet_TCP_GetPeerAddress(client);
//	if(!remoteip)
//	{
//		//undone printf("SDLNet_TCP_GetPeerAddress: %s\n",SDLNet_GetError());
//	}
//
//	if(memcmp(&remoteip->host,allowedIpInBE,sizeof remoteip->host)){
//		SDLNet_TCP_Close(client);
//		//undone do some reporting about this unexpected connection?
//		return;
//	}
//
//}
//
//static void closeEstablishedConn(void){
//	SDLNet_TCP_Close(client);
//}

