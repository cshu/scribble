//#include "keymappedtofile.h"
#define NO_NEED_TO_AVERT_RACES
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "cpprs.h"
#include "httpserverforipc.h"
#include "cpprs_stdout_as_log.h"

#include "independent_rt_portnumbers.h"

int main (int argc, char **argv){
	//files_dir [port_num] (if port num is provided then use it instead of default port)
#define KEYSNFILENAMES_FILE "0"
#define FILES_DIR (argv[1])
	portable_rt_port_int_t http_port_num;
	switch(argc){
		case 2:
			http_port_num=TCP_PORT_KEYMAPPEDTOFILE_APP;
			break;
		case 3:
			STRTOUL_WITHOUT_ENDPTRu(parsePort_,http_port_num,argv[2],10,{puts("Parseing port num failed"); return RETVALFAILURE;},)
			break;
		default:
			puts("Usage: dir_path [port_num]");
			return RETVALFAILURE;
	}
	size_t strlenofFilesdir = strlen(FILES_DIR);
	char *filenmbuf = malloc(strlenofFilesdir+COMMON_MAX_FILENAME_LENGTH+16);
	if(!filenmbuf){LOG_ERROR_LITERALs("Memory allocation failed.") return RETVALFAILURE;}
	MEMCPY_IF_LEN(filenmbuf,FILES_DIR,strlenofFilesdir);
	int retintbuf;
	int errintbuf;
	unsigned long long numOfFiles;
	COUNT_NUM_OF_FILESu(count_stored_,FILES_DIR,numOfFiles,{
		puts("failed when counting num of files."); goto start_of_fail_freefilenmbuf;
	},)

	char *keysnfilenames;
	size_t keysnfilenamesSi;
	READ_WHOLE_FILE_WITH_NUL_APPENDEDu(readkeysnnames_,keysnfilenames,keysnfilenamesSi,KEYSNFILENAMES_FILE,{
		LOG_ERROR_LITERALs("io failed")
		goto start_of_fail_freefilenmbuf;
	},{
		LOG_ERROR_LITERALs("empty file, not ended with lf")
		goto start_of_fail_freefilenmbuf;
	},);
	size_t off_keysnfilenmbuf=0;
	for(;;){
		off_keysnfilenmbuf += strspn(keysnfilenames+off_keysnfilenmbuf,"\n");
		if(off_keysnfilenmbuf==keysnfilenamesSi) break;
		char *poff_keysnfilenmbu = strchr(keysnfilenames+off_keysnfilenmbuf,'\n');
		if(!poff_keysnfilenmbu){puts("cannot find the ending lf for key"); goto start_of_fail_freekeys;}
		off_keysnfilenmbuf = poff_keysnfilenmbu-keysnfilenames+1;
		poff_keysnfilenmbu = strchr(keysnfilenames+off_keysnfilenmbuf,'\n');
		if(!poff_keysnfilenmbu){puts("cannot find the ending lf for filename"); goto start_of_fail_freekeys;}
		poff_keysnfilenmbu[0]=0;
		ENSURE_DIR_OR_OTHER_KIND_OF_FILESYSTEM_ENTRY_EXISTSu(ensureFileExist_,(keysnfilenames+off_keysnfilenmbuf),{fputs("failed to find: ",stdout);puts(keysnfilenames+off_keysnfilenmbuf); goto start_of_fail_freekeys;},,{puts("dir found"); goto start_of_fail_freekeys;})
		poff_keysnfilenmbu[0]='\n';
		--numOfFiles;
		off_keysnfilenmbuf = poff_keysnfilenmbu-keysnfilenames+1;
	}
	if(numOfFiles!=1) {puts("num of files doesn't match data in keys & filenames file."); goto start_of_fail_freekeys;}

	PORTABLE_RT_SO_INITs(retintbuf,{LOG_ERROR_INTs(retintbuf)
		goto start_of_fail_freekeys;},)
	portable_rt_socket_t listensock;
	LISTEN_INADDR_LOOPBACK_WITH_PORT_IN_HOST_BYTE_ORDERu(listen_,listensock,http_port_num,{LOG_ERROR_LITERALs("Invalid socket.")
		goto start_of_fail_so;
	},{LOG_ERROR_LITERALs("Bind error.")
		goto start_of_fail_so;
	},{LOG_ERROR_LITERALs("Listen error.")
		goto start_of_fail_so;
	},)
	SET_NONBLOCK_MODE_FOR_SOCKETu(listenNonBlock_,listensock,{LOG_ERRORs goto start_of_fail_so;},)//fixme need to close listensock
	struct pvoid_with_size_n_capacity fds;
	SET_SI_MALLOC_PVOID_WITH_SI_CAs(fds.,sizeof(portable_rt_socket_t)*3,{
		LOG_ERROR_LITERALs("malloc fds failed") goto start_of_fail;//fixme no need to free fds.
	},)
	APPEND_TO_PVOID_WITH_SI_CA_N_REALLOC_IF_NEEDEDu(appendListenSock_,fds.,listensock,portable_rt_socket_t *,AT_LEAST_ONE_N_HALF_OF_X_AS_MULTIPLE_OF_Y,{
		LOG_ERROR_LITERALs("append fd failed") goto start_of_fail;
	},)
	const struct timeval accepttv={.tv_sec=60, .tv_usec=0};
	const struct timeval singlerecvtv={.tv_sec=16, .tv_usec=0};
	fd_set mask;
	start_select:{
		SELECT_SINGLE_READABLE_FD_WITH_COPIED_TIMEVAL_N_CHECKs(listensock,&mask,accepttv,{
			retintbuf = PORTABLE_RT_SO_GET_ERROR_INT_OR_ERRNO;
			LOG_ERROR_INTs(retintbuf)
			goto start_of_fail;
		},{
			//todo add checking (stdin/file) for request of saving and exiting (in case socket request somehow doesn't work)
			goto start_select;
		},{goto start_accept;})
	}
	for(;;){
		//undone add fds
		//undone select with infinite timeout
		if(1){//if listensock
			start_accept:;
			struct sockaddr_in cli;
			portable_rt_socket_t clisock;
			ACCEPT_WITH_CLIADDR_N_SET_NONBLOCK_MODEu(acceptCli_,clisock,listensock,cli,{LOG_ERRORs /*//undone goto select on all sockets again*/},)
			
			APPEND_TO_PVOID_WITH_SI_CA_N_REALLOC_IF_NEEDEDu(appendCliSock_,fds.,clisock,portable_rt_socket_t *,AT_LEAST_ONE_N_HALF_OF_X_AS_MULTIPLE_OF_Y,{
				LOG_ERROR_LITERALs("append fd failed")
				//undone goto select on all sockets again
			},)

			//note actually you can write `continue` here, but normally this new client should send something immediately so select it alone for once.

			//undone start processing one transaction!
	//			SELECT_SINGLE_READABLE_FD_WITH_COPIED_TIMEVAL_N_CHECKs(clisock,&mask,singlerecvtv,{
	//				//undone socket error
	//			},{
	//				//undone go back, select() on all sockets again
	//			},)
	//			//undone recv uuid and check
	//			//undone recv and check
			char *reqBody;
			size_t receivedsi;
			//recvHttpRequest();
			RECV_HTTP_REQUEST_N_RETAIN_BODY_WITH_COPIED_TIMEVALu(recvbody_,reqBody,receivedsi,clisock,&mask,singlerecvtv,{/*//undone*/},{/*//undone*/},)
			//debugLog(reqBody,recvoff);
			FOPEN_WB_FWRITE_FCLOSEu(writelog_,reqBody,recvoff,"outhttpdump.txt",{puts("writing failed");},)
			SEND_HTTP_RESPONSE_HEADERS_WITH_CL_INTEGER_N_COPIED_TIMEVALu(httpResp_,clisock,&mask,singlerecvtv,13,{/*//undone*/},{/*//undone*/},)
			SELECT_N_SEND_ALL_WITH_COPIED_TIMEVALu(sendbody_,"chr\xE5\x8D\x80 in chn")
			puts("debug ALL DONE");
			BERKELEY_SO_CLOSE(clisock);
			BERKELEY_SO_CLOSE(listensock);

			goto start_of_fail;
			//
			//memcmp()
			//undone select to wait for data

			//undone read req
			//
			//RECV_WITHOUT_FLAGS(clisock,pbuf,len)
			//
		}else{
			//undone start processing one transaction!
		}
	}
	//MODULEUUIDSTR_KEYMAPPEDTOFILE_APP_SERVER

	
	//undone listen on a port, accept conn




	//undone write_to_KEYSNFILENAMES_FILE if needed
	FREE_PVOID_WITH_SI_CA(fds.);
	PORTABLE_RT_SO_QUITs(retintbuf,errintbuf)
	if(errintbuf){LOGERR_INT(errintbuf)}
	if(retintbuf){LOG_ERROR_INTs(retintbuf) free(keysnfilenames); free(filenmbuf); return RETVALFAILURE;}
	free(keysnfilenames);
	free(filenmbuf);
	return RETVALSUCCESS;
	
	start_of_fail:
	FREE_PVOID_WITH_SI_CA(fds.obj);
	start_of_fail_so:
	PORTABLE_RT_SO_QUITs(retintbuf,errintbuf)
	if(errintbuf){LOGERR_INT(errintbuf)}
	if(retintbuf){LOG_ERROR_INTs(retintbuf) free(keysnfilenames); free(filenmbuf); return RETVALFAILURE;}
	start_of_fail_freekeys:
	free(keysnfilenames);
	start_of_fail_freefilenmbuf:
	free(filenmbuf);
	return RETVALFAILURE;
}



