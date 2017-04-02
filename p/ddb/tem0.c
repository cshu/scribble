




static void errorLogCallback(void *pArg, int iErrCode, const char *zMsg) {
	LOG_ERROR_INT_N_STRs(iErrCode,zMsg)
}
int main (int argc, char **argv){
	int intbuf, errintbuf;
//undone read config file
//port
//listen any or only local or what
//sqlite filename
//folders/files for unique filename file io extension. When ddb needs to access a file with unique allocated name, only search in these folders or files (note you can also add folders/files while ddb is running, in case e.g. you connected an external drive after starting ddb.)
//folders for new file creation or moving unregistered files. Different extensions may use different folders. Note this can also be set when ddb is running.
	portable_rt_port_int_t portnum;
	STRTOUL_WITHOUT_ENDPTRu(parsePort_,portnum,"80",10,{puts("Parseing port num failed"); return ERRCODE_ERR;},)//undone
	portnum=htons(portnum);
	portable_rt_addr_int_t addrnum=htonl(INADDR_ANY);//undone
	char *utf8dbfnm = "C";//undone
	filesystem_name shortnotedir;
	filesystem_name audioproddir;
	filesystem_name strmappedtofiledir;
	GET_FILESYSTEM_NAME_WITH_UTF8STRu(getfsnm_,"sns",shortnotedir,{puts("sns dir, something wrong"); return ERRCODE_ERR;},)//undone
	GET_FILESYSTEM_NAME_WITH_UTF8STRu(getfsnm_,"audioproducts",shortnotedir,{puts("audio dir, something wrong"); return ERRCODE_ERR;},)//undone
	GET_FILESYSTEM_NAME_WITH_UTF8STRu(getfsnm_,"strmappedtofiles",shortnotedir,{puts("strmappedtofiles dir, something wrong"); return ERRCODE_ERR;},)//undone


	PORTABLE_RT_SO_INITs(intbuf,{LOG_ERROR_INTs(intbuf) goto freefsnms;}//fixme goto uninit sqlite?
	portable_rt_socket_t listensock;
	LISTEN_INADDR_LOOPBACK_WITH_PORT_IN_HOST_BYTE_ORDERu(listen_,listensock,http_port_num,{LOG_ERROR_LITERALs("Invalid socket.")
		goto quitso;
	},{LOG_ERROR_LITERALs("Bind error.")
		goto quitso;
	},{LOG_ERROR_LITERALs("Listen error.")
		goto quitso;
	},)
	SET_NONBLOCK_MODE_FOR_SOCKETu(listenNonBlock_,listensock,{LOG_ERRORs goto closelisten;},)
	struct pvoid_with_size_n_capacity fds;
	SET_SI_MALLOC_PVOID_WITH_SI_CAs(fds.,sizeof(portable_rt_socket_t)*3,{
		LOG_ERROR_LITERALs("malloc fds failed") goto closelisten;
	},)
	APPEND_TO_PVOID_WITH_SI_CA_N_REALLOC_IF_NEEDEDu(appendListenSock_,fds.,listensock,portable_rt_socket_t *,AT_LEAST_ONE_N_HALF_OF_X_AS_MULTIPLE_OF_Y,{
		LOG_ERROR_LITERALs("append fd failed") goto freefds;
	},)
	const struct timeval accepttv={.tv_sec=60, .tv_usec=0};
	const struct timeval singlerecvtv={.tv_sec=16, .tv_usec=0};
	fd_set mask;

	char *zErrMsg;
	intbuf = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto freefds;}
	intbuf = sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, NULL);
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto freefds;}
	intbuf = sqlite3_open(utf8dbfnm, &databaseA);
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto freefds;}
	intbuf = sqlite3_exec(databaseA, "PRAGMA locking_mode = EXCLUSIVE", NULL, NULL, &zErrMsg);
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto closedb;}
	intbuf = sqlite3_exec(databaseA, "PRAGMA synchronous = OFF", NULL, NULL, &zErrMsg);
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto closedb;}
	intbuf = sqlite3_exec(databaseA, "PRAGMA journal_mode = OFF", NULL, NULL, &zErrMsg);
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto closedb;}
	intbuf = sqlite3_exec(databaseA, "PRAGMA cache_size = 90000000", NULL, NULL, &zErrMsg);//90GB in-memory (with 1024 as page_size)
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto closedb;}
	intbuf = sqlite3_exec(databaseA, "PRAGMA temp_store = MEMORY", NULL, NULL, &zErrMsg);
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto closedb;}
	intbuf = sqlite3_exec(databaseA, "PRAGMA automatic_index = 0", NULL, NULL, &zErrMsg);//main reason to turn this off is that my WITH clause causes (284) automatic index warning!
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto closedb;}
	intbuf = sqlite3_exec(databaseA, "BEGIN EXCLUSIVE", NULL, NULL, &zErrMsg);
	if (SQLITE_OK != intbuf) {LOG_ERROR_INTs(intbuf) goto closedb;}

	for(;;){
		//
	}
	//undone select


	




	commitnclosedb:
	//undone if not saved, then
	//undone finalize all stmt
	intbuf = sqlite3_exec(databaseA, "COMMIT", NULL, NULL, &zErrMsg);
	if(SQLITE_OK!=intbuf){LOG_ERROR_INTs(intbuf)}
	closedb:
	intbuf = sqlite3_close(databaseA);
	if(SQLITE_OK!=intbuf){LOG_ERROR_INTs(intbuf)}

	freefds:
	FREE_PVOID_WITH_SI_CA(fds.);
	closelisten:
	BERKELEY_SO_CLOSE(listensock);
	quitso:
	PORTABLE_RT_SO_QUITs(intbuf,errintbuf)
	
	freefsnms:
	FREE_GOT_FILESYSTEM_NAMEs(strmappedtofiledir)
	FREE_GOT_FILESYSTEM_NAMEs(audioproddir)
	FREE_GOT_FILESYSTEM_NAMEs(shortnotedir)
	return 0;
}

//note when you do an operation that needs to access a file, and that file is not found, that operation fails. Warn user that peripheral data source is not available.
//note when you create new files or moving unregistered files to folder for registration, you might need to ask user for confirmation about which location to use.
//note client side may pass the file path/uri to server, or it may directly transfer file content to server

//feature: check files exist, ddb checks all registered filenames to ensure that all of them exists and file attributes match db records. Also warn user when unregistered filename is found in folders/files.

