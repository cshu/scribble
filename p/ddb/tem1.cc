
//undone placeholder use only
#define LOG_ERROR_INT_N_STRs(x,y)
#define THROW {throw 0;}
#define LOG_ERR {}//undone use c++ template for logging int, const char *, etc.

#include<csetjmp>

#include<sqlite3minwrapper.h>

struct glstruct{
	sqlite3 *ddb;
};

union uuty{
	struct glstruct glst;
};




struct structforjb{
	jmp_buf env;
	void (*f)(struct structforjb *,union uuty *);//?separate func pointer this, not bundled with jmp_buf, for better performance?
};
#ifdef __cplusplus
template<class T>
#else
#define T union uuty *
#endif
void mainfuncloop(void (*func)(struct structforjb *,T),T punion){
	
	struct structforjb jb;
	jb.f=func;
	struct structforjb *pjb=&jb;

	switch(setjmp(jb.env)){
	case 0:
		for(;;){jb.f(pjb,punion);}
		break;
	default:
		break;
	}

}
void funclj(struct structforjb *jb, union uuty *punion){
	longjmp(jb->env,1);
}




struct errnsuc{
	void (*err)(struct structforjb *,union utty *);
	void (*suc)(struct structforjb *,union utty *);
};


/******  ******/
/******sqlite physical layer******/
struct st_initsl{
	void (*err)(struct structforjb *,union utty *);
	void (*suc)(struct structforjb *,union utty *);
};
void initsl(struct structforjb *jb, union uuty *punion){
	//
	jb->f=punion->initsl.suc;
}

void commitnclosesl(struct structforjb *jb, union uuty *punion){
	intbuf = sqlite3_exec(databaseA, "COMMIT", NULL, NULL, &zErrMsg);
	if(SQLITE_OK != intbuf){
	}
	intbuf = sqlite3_close(databaseA);
	if(SQLITE_OK != intbuf){
	}
}

void init_sl(){
	int ibuf;
	ibuf = sqlite3_config(SQLITE_CONFIG_SINGLETHREAD);
	if(SQLITE_OK != ibuf){THROW(ibuf)}
	ibuf = sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, NULL);
	if(SQLITE_OK != ibuf){THROW(ibuf)}
	//
}
static sqlite3_int64 greatestid;
sqlite3_int64 ret_avail_globalid()noexcept{
	++greatestid;
	return greatestid;
}
sqlite3_int64 select_greatestid(){
	int ibuf;
	sqlite3_stmt *stmt;
	ibuf = sqlite3_prepare_v2(databaseA, "SELECT v FROM memoizedcfg_i WHERE k=? LIMIT 1", -1, &stmt, NULL);
	if(ibuf!=SQLITE_OK) {THROW(ibuf)}
	slw_stmt wstmt{stmt};
	ibuf = sqlite3_bind_blob(stmt, 1, LITERALcommaLEN("greatest_id"), SQLITE_STATIC);
	if(ibuf!=SQLITE_OK) {THROW(ibuf)}
	ibuf = sqlite3_step(stmt);
	if(retintbuf != SQLITE_ROW) {THROW(ibuf)}
	return sqlite3_column_int64(stmt, 0);
}
//struct ret_avail_globalid_st(
//	f;
//	slintbuf;
//};
//void ret_avail_globalid(struct structforjb *jb, void *obj){
//	static sqlite3_int64 availid;
//	if(availid){
//		obj->slintbuf=availid++;
//		jb->f=obj->f;
//	}else{
//	}
//}
/******  ******/

//undone a way to implement punion: each build_ func has its own (or some funcs share the same) struct, which has a pointer pointing to underlying struct, which in turn has a pointer pointing to lower struct, until it reaches the lowest struct allocated in the begining of func loop.

void build_main(struct structforjb *jb, void *obj){
	//undone read stem_argv, load core and peripheral db
	//undone set availid
	//undone suppose filesystem extension is needed
	//undone suppose some common extensions are needed

	jb->f=initsl;
	//undone setting parameters for initsl
}
//void errh_func
void build_shutdownddb(struct structforjb *jb, union uuty *punion){
	jb->f=commitnclosesl;
}

static void errorLogCallback(void *pArg, int iErrCode, const char *zMsg) {
	LOG_ERROR_INT_N_STRs(iErrCode,zMsg)
}
static char **stm_argv;
static int stm_argc;
int main(int argc, char **argv){
	//note this is the entry point of CLIENT program!
	stm_argc=argc;
	stm_argv=argv;
	int intbuf, errintbuf;

	union uuty u;
	mainfuncloop(build_main,&u);
}
