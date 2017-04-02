#pragma once


#include "httpserverforipc.h"



static void servehttp(void){
	init_httpserverforipc();//undone check success

	startListening();
	acceptConn(strLocalhostInBE);
	//undone recv request content
	//undone repond http reponse
	//undone loop
	//undone if conn closed by client

	quit_httpserverforipc();
}


static void sendReqToStopServer(IPaddress ip){
	sock=SDLNet_TCP_Open(&ip);
	if(!sock){
		//undone printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
	}

	result=SDLNet_TCP_Send(sock,HTTPSERVERFORIPC_STREXIT,sizeof(HTTPSERVERFORIPC_STREXIT)); /* add 1 for the NULL */
	if(result<sizeof(HTTPSERVERFORIPC_STREXIT)){
		//undone printf("SDLNet_TCP_Send: %s\n",SDLNet_GetError());
	}
	unsigned char ucbuf;
	result=SDLNet_TCP_Recv(sock,&unbuf,1);
	if(result<=0){
		//undone error/remote side closed conn.
	}
	if(memcmp(HTTPSERVERFORIPC_STRCONFIRMEXIT,&ucbuf,1)){
		//undone ack received, but the content is not as expected.
	}
	SDLNet_TCP_Close(sock);
}


