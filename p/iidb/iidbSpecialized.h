
#include "griddoor.h"

#define MAXFILENAMELEN 12



//undone values here!
//IS_A,
//IS_A_SHORTNOTE_ARG,
//AN_ARG_ID_TO_BE_EXPLAINED_BY_UTF8_NATURAL_LANGUAGE_TEXT,

//note here are some constant sense ids
// #define GENDER_MALE 65541
// #define GENDER_FEMALE 65542
// #define NATURALLANG_ENGLISH 65543
// #define NATURALLANG_CHINESE 65544
// #define NATURALLANG_SIMPLIFIEDCHINESE 65545
// #define NATURALLANG_TRADITIONALCHINESE 65546
// #define NATURALLANG_JAPANESE 65547
// #define NATURALLANG_RUSSIAN 65548

//note here are some constant verbs
//note! this value is not inside your in_singleargv table!
#define IS_A_NATURAL_LANG_WORD_SENSE 70001


//note here are some defined byte array TYPES:
//utf-8 text
//non-negative integer (Little-endian) without length limit
//gender single byte 0 for female, 1 for male, simply in ascii order
//progress bar byte array, non-negative integer (Little-endian) without length limit, 0 for finished, 1 for not started, bigger integer for bigger progress
//progress bar specialized byte array (e.g. 1. anime episode number/half of an episode, 2. fiction reading byte index)
//ranking byte array, any data is okay, used for memcmp ranking
//ranking byte array with meaningful distance (score), e.g. {21,131,25} is very close to {21,131,27} but very far from {23,131,25}

static int tempSpDeployment(void){
    
    int retintbuf;
    char *zErrMsg = NULL;
    
    //debug block
//    sqlite3_stmt *xpssbuf;
//    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select v,k from memoizedcfg_i where k like ?1 escape ?2"), &xpssbuf, NULL);
//    LOGERRs
//    char chBuf = '`';
//    retintbuf = sqlite3_bind_text(xpssbuf, 2, &chBuf, 1, SQLITE_TRANSIENT);
//    LOGERRs
//    retintbuf = sqlite3_bind_text(xpssbuf, 1, "%`_%", -1, SQLITE_TRANSIENT);
//    LOGERRs
//    stepag:
//    retintbuf = sqlite3_step(xpssbuf);
//    if(retintbuf != SQLITE_ROW){
//        LOGERRs
//        SLFINALIZEs(xpssbuf)
//        return 0;
//    }
//    sqlite3_int64 sibufx = sqlite3_column_int64(xpssbuf, 0);
//    printf("\n%lld\n",sibufx);
//    goto stepag;
//    return 0;
    
    //debug block
//    char sqltore[]= "insert into memoizedcfg_i values (                      ,x'304050607023344512324135')";
//    char sin[10]={'a'};
//    for(int i=1;;i++){
//    itoa(i,sqltore+sizeof "insert into memoizedcfg_i values (");
//    retintbuf = sqlite3_exec(databaseA, sqltore, NULL, NULL, &zErrMsg);
//    if(zErrMsg){
//        LOGMESSAGEs(zErrMsg)
//        LOGERRs
//        goto fail;
//    }
//    if(i%131072==0){
//    scanf("%10s", sin);
//    if(sin[0]=='b')
//        break;
//    }
//    }
//    return 0;
    
    retintbuf = sqlite3_exec(databaseA, "create table in_entailment(a integer,c integer,primary key(a,c))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    retintbuf = sqlite3_exec(databaseA, "create table in_mutexgroup(g integer,v integer,primary key(g,v))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    retintbuf = sqlite3_exec(databaseA, "create table in_singleargv(v integer,d blob,primary key(v,d))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    retintbuf = sqlite3_exec(databaseA, "create table in_singleargp(v integer,s integer,primary key(v,s))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    retintbuf = sqlite3_exec(databaseA, "create table sp_shortnotes(s integer,t blob,primary key(s,t))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //short note serving as a comment of some thing(s)
    retintbuf = sqlite3_exec(databaseA, "create table sp_sncomments(s integer,i integer,t blob,primary key(s,i,t))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    retintbuf = sqlite3_exec(databaseA, "create table sp_urlbookmar(i integer,u blob,d blob,primary key(i,u,d))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //u means url OR description (when it's not a web address), a means group id, for linking to strings (and maybe some verbs)
    retintbuf = sqlite3_exec(databaseA, "create table sp_autheninfo(a integer,u blob,primary key(a,u))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //g means sense id, t means text, this table is used for essential text senses associated with a sense, e.g. authinfo strings.
    retintbuf = sqlite3_exec(databaseA, "create table sp_essenttext(g integer,t blob,primary key(g,t))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //prog is progress bar blob
    //rank is a ranking blob for personal ranking.
    //comm is comment about it.
    retintbuf = sqlite3_exec(databaseA, "create table sp_animationp(i integer,prog blob,rank blob,comm blob,primary key(i,prog,rank,comm))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //note the i might also exists in sp_animationp
    retintbuf = sqlite3_exec(databaseA, "create table sp_myanimelis(i integer,mid blob,title blob,english blob,synonyms blob,episodes blob,type blob,primary key(i,mid,title,english,synonyms,episodes,type))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //sense id, storage (unique?) filename, size in bytes, 40-bit last write time, original filename
    retintbuf = sqlite3_exec(databaseA, "create table sp_filestorag(i integer,snm blob,size blob,wtime blob,onm blob,primary key(i,snm,size,wtime,onm))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //set id and and member id, the set contains exactly these members!
    retintbuf = sqlite3_exec(databaseA, "create table sp_setexsense(s integer,m integer,primary key(s,m))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //set id and and member id, the set might contain more!
    retintbuf = sqlite3_exec(databaseA, "create table sp_setcosense(s integer,m integer,primary key(s,m))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //d for data sense (digital file, tangible medium, etc.), FULLY storing information sense i.
    //d can be a collection. Although you don't have to force yourself use this table, you can always choose sp_datastinfo
    retintbuf = sqlite3_exec(databaseA, "create table sp_datafsinfo(d integer,i integer,primary key(d,i))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //d for data sense (digital file, tangible medium, etc.), storing information sense i.
    retintbuf = sqlite3_exec(databaseA, "create table sp_datastinfo(d integer,i integer,primary key(d,i))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //personified identity table, use sp_essenttext for canonical names, real names etc.
    //gender, date when born, date when died, comment text
    retintbuf = sqlite3_exec(databaseA, "create table sp_personifie(i integer,g blob,dborn blob,ddied blob,comm blob,primary key(i,g,dborn,ddied,comm))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //personified identity (or set of persons), author/writer/creator (collaborative), information (publication/art)
    retintbuf = sqlite3_exec(databaseA, "create table sp_authorinfo(p integer,i integer,primary key(p,i))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //personified identity (or set of persons), translator, information (publication using specific natural language)
    retintbuf = sqlite3_exec(databaseA, "create table sp_translinfo(p integer,i integer,l integer,primary key(p,i,l))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //mainly textual fiction (additional multi-media is allowed, e.g. light novel)
    //prog is progress bar blob
    //rank is a ranking blob for personal ranking.
    //comm is comment about it.
    retintbuf = sqlite3_exec(databaseA, "create table sp_mtextficti(i integer,prog blob,rank blob,comm blob,primary key(i,prog,rank,comm))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    
    //note you might not need the table StorageDevice-storing-File, because you have setcosense table and then a single-arg verb "all senses in X are stored in Gray Maxtor"
    //so it's not very necessary unless you have hundreds of storage devices
    
    //mainly audio product (slightly other media is allowed, e.g. a background image)
    //a for artist(s)/studio
    //rank is a ranking blob for personal ranking.
    retintbuf = sqlite3_exec(databaseA, "create table sp_audioprodu(i integer,a integer,rank blob,primary key(i,a,rank))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //compile,embody,include,collect (e.g. anthology,audio album,mixture product of different publications)
    //a kind of info-info relationship
    //note this can hardly be replaced with set-member verb because you'll still need another verb for album-set
    //a pure set cannot be an audio album itself (a pure set cannot be associated with essential text or other properties like year, publisher)
    //i is the collection's id, m is member id, d is date(time) of publishing/appearing, t is title/description/text
    retintbuf = sqlite3_exec(databaseA, "create table sp_compileemb(i integer,m integer,d blob,t blob,primary key(i,m,d,t))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //video games
    //a for author/studio
    //rank is a ranking blob for personal ranking.
    retintbuf = sqlite3_exec(databaseA, "create table sp_videogames(i integer,a integer,rank blob,primary key(i,a,rank))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    //note you should rarely need product series table here, because you can have sp_setcosense for comprising all members, and then with a single-arg verb "X is a product series"
    //you might need it when you have very special product series which requires practical properties, like a special name as essential text
    
    //propositions of table:
    //time(date) of appearance table, for ANY sense first appearing in the universe.
    
    retintbuf = sqlite3_exec(databaseA, "create table memoizedcfg_i(v integer,k blob,primary key(v,k))without rowid", NULL, NULL, &zErrMsg);
    if(zErrMsg){
        LOGMESSAGEs(zErrMsg)
        LOGERRs
        goto fail;
    }
    if (ISRETVALOFSQLITEEXECERR){
        LOGERRs
        goto fail;
    }
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into memoizedcfg_i values(262144,?)"), &pssbuf, NULL);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        goto fail;
    }
    retintbuf = sqlite3_bind_blob(pssbuf, 1, LITERALcommaLEN("AVAIL_SENSE_ID"), SQLITE_STATIC);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        goto fail;
    }
    retintbuf = sqlite3_step(pssbuf);
    if (retintbuf != SQLITE_DONE) {
        LOGERRs
        goto fail;
    }
    SLFINALIZEs(pssbuf)
    return RETVALSUCCESS;
    
    fail:
    return RETVALFAILURE;
}

static int tempSpExecArbitrarySQLAndRetData(void){
    //undone mainly for multi step filtering
    //undone 1.data var 2.prepare sql 3.bind 4.get col data with type
    
    //"select sp_shortnotes.s,t,in_singleargv.v,d from sp_shortnotes left join in_singleargp on sp_shortnotes.s=in_singleargp.s left join in_singleargv on in_singleargp.v=in_singleargv.v where t like ? escape ? order by sp_shortnotes.s" " AND/OR t like blablaba" " and exists(select * from in_singleargp where v=? and s=sp_shortnotes.s)" "AND/OR blablabla"
    
    //
    return 0;
}

static int tempSpExecArbitrarySQL(void){
    int retintbuf;
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, (char *)pucBuf24, len, &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    int bindInd = 1;
    sqlite3_int64 sibuf;
    char chBuf;
    while(1){
        retintbuf = RECVWITHVAR(chBuf);
        IF_ISRETVALERR_LOGNRETURNs
        switch(chBuf){
            case 0:
                goto endofbinding;
            case 1:
                retintbuf = RECVWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_bind_int64(pssbuf, bindInd++, sibuf);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                break;
            case 2:
                retintbuf = recvWithUcBuf24(&len);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_bind_text(pssbuf, bindInd++, (char *)pucBuf24, len, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                retintbuf = RECVWITHVAR(chBuf);
                IF_ISRETVALERR_LOGNRETURNs
                if(chBuf) retintbuf = sqlite3_bind_text(pssbuf, bindInd++, &chBuf, 1, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                break;
            case 3:
                retintbuf = recvWithUcBuf24(&len);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_bind_blob(pssbuf, bindInd++, pucBuf24, len, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                break;
        }
    }
    endofbinding:;
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf != SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, 1);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    SLFINALIZEs(pssbuf)
    return RETVALSUCCESS;
}

static int tempSpUrlBookmarkVSearch(void){
    int retintbuf;
    unsigned char *ucbufter=pucBuf24;
    MEMCPYLITERALnADVANCEs(ucbufter, "select * from sp_urlbookmar where ")
    uint_least32_t len;
    retintbuf = recvUnsigned24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = recvUntilBytes(ucbufter, len);
    IF_ISRETVALERR_LOGNRETURNs
    ucbufter[len]=0;
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, (char *)pucBuf24, -1, &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    int bindInd = 1;
    sqlite3_int64 sibuf;
    char chBuf;
    while(1){
        retintbuf = RECVWITHVAR(chBuf);
        IF_ISRETVALERR_LOGNRETURNs
        switch(chBuf){
            case 0:
                goto endofbinding;
            case 1:
                retintbuf = RECVWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_bind_int64(pssbuf, bindInd++, sibuf);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                break;
            case 2:
                retintbuf = recvWithUcBuf24(&len);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_bind_text(pssbuf, bindInd++, (char *)pucBuf24, len, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                retintbuf = RECVWITHVAR(chBuf);
                IF_ISRETVALERR_LOGNRETURNs
                if(chBuf) retintbuf = sqlite3_bind_text(pssbuf, bindInd++, &chBuf, 1, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                break;
        }
    }
    endofbinding:;
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 2);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 2), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if (retintbuf != SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpShortNotesVSearch(void){
    int retintbuf;
    unsigned char *ucbufter=pucBuf24;
    MEMCPYLITERALnADVANCEs(ucbufter, "select sp_shortnotes.s,t,in_singleargv.v,d from sp_shortnotes left join in_singleargp on sp_shortnotes.s=in_singleargp.s left join in_singleargv on in_singleargp.v=in_singleargv.v where ")
    uint_least32_t len;
    retintbuf = recvUnsigned24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = recvUntilBytes(ucbufter, len);
    IF_ISRETVALERR_LOGNRETURNs
    memcpy(ucbufter+len,LITERALcommaSIZE(" order by sp_shortnotes.s"));
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, (char *)pucBuf24, -1, &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    int bindInd = 1;
    sqlite3_int64 sibuf;
    char chBuf;
    while(1){
        retintbuf = RECVWITHVAR(chBuf);
        IF_ISRETVALERR_LOGNRETURNs
        switch(chBuf){
            case 0:
                goto endofbinding;
            case 1:
                retintbuf = RECVWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_bind_int64(pssbuf, bindInd++, sibuf);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                break;
            case 2:
                retintbuf = recvWithUcBuf24(&len);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_bind_text(pssbuf, bindInd++, (char *)pucBuf24, len, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                retintbuf = RECVWITHVAR(chBuf);
                IF_ISRETVALERR_LOGNRETURNs
                if(chBuf) retintbuf = sqlite3_bind_text(pssbuf, bindInd++, &chBuf, 1, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                break;
        }
    }
    endofbinding:;
    sqlite3_int64 lastid = 0;
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW){
            if(retintbuf != SQLITE_DONE){
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
            }
            if(lastid!=0){
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64)*2);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
            }else{
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
            }
            break;
        }
        sibuf = sqlite3_column_int64(pssbuf, 0);
        if(lastid == sibuf){
            goto startOfRetVerb;
        }else if(lastid!=0){
            retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
        }
        lastid = sibuf;
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        if(blen){
            const char *cv = sqlite3_column_blob(pssbuf, 1);
            char *nln = memchr(cv, '\n', blen);
            char *nlr = memchr(cv, '\r', blen);
            if(nln){
                if(nlr)
                    blen = MINOFTWO(nln,nlr)-cv;
                else
                    blen = nln-cv;
                SENDLEN24WITHLENCHKs(cv, blen);
            }else if(nlr){
                blen = nlr-cv;
                SENDLEN24WITHLENCHKs(cv, blen);
            }else
                retintbuf = sendLen24(cv, blen);
        }else
            retintbuf = sendUntilBytes(SEVENEXPLICITNULLITERAL, 3);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        startOfRetVerb:
        retintbuf=sqlite3_column_type(pssbuf,2);
        switch(retintbuf){
            case SQLITE_NULL:
                break;
            case SQLITE_INTEGER:
                sibuf = sqlite3_column_int64(pssbuf, 2);
                retintbuf = SENDWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                blen = sqlite3_column_bytes(pssbuf, 3);
                SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 3), blen);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                break;
            default:
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
        }
    }
    SLFINALIZEs(pssbuf)
    return RETVALSUCCESS;
}

static int tempSpSearchVerbDesc(void){
    int retintbuf;
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    char chBuf;
    retintbuf = recvUntilBytes(&chBuf, 1);
    IF_ISRETVALERR_LOGNRETURNs
    unsigned char whflgs;
    retintbuf = RECVWITHVAR(whflgs);
    IF_ISRETVALERR_LOGNRETURNs
#define SELSL "select * from in_singleargv where d like ?"
#define EXTWHST " and v in(select v from in_singleargp where s in ("
#define EXTWHEN "))"
#define SNCL "select s from sp_shortnotes union all "
#define ANCL "select i from sp_animationp union all select i from sp_myanimelis union all "
#define WSCL "select s from in_singleargp where v=" XSTR(IS_A_NATURAL_LANG_WORD_SENSE) " union all "
#define UBCL "select i from sp_urlbookmar union all "
    char *slbuf = malloc(sizeof SELSL+sizeof " escape ?"+sizeof EXTWHST+sizeof EXTWHEN+sizeof SNCL+sizeof ANCL+sizeof WSCL+sizeof UBCL);
    if(!slbuf){LOGLITERALs("memalloc failed") return RETVALMEMALLOCFAIL;}
    if(chBuf) memcpy(slbuf, LITERALcommaSIZE(SELSL " escape ?")); else memcpy(slbuf, LITERALcommaSIZE(SELSL));
    if(whflgs){
        strcat(slbuf,EXTWHST);
        if(whflgs&1) strcat(slbuf,SNCL);
        if(whflgs&2) strcat(slbuf,ANCL);
        if(whflgs&4) strcat(slbuf,WSCL);
        if(whflgs&8) strcat(slbuf,UBCL);
        strcpy(slbuf+strlen(slbuf)-(sizeof" union all "-1),EXTWHEN);
    }
#undef SELSL
#undef EXTWHST
#undef EXTWHEN
#undef SNCL
#undef ANCL
#undef WSCL
#undef UBCL
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, slbuf, -1, &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    if(chBuf){
        retintbuf = sqlite3_bind_text(pssbuf, 2, &chBuf, 1, SQLITE_TRANSIENT);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    }
    retintbuf = sqlite3_bind_text(pssbuf, 1, (char *)pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        sqlite3_int64 sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if (retintbuf != SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpCreateV(void){
    int retintbuf;
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_int64 sibuf;
    retintbuf = retAvailGlobalId(&sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_singleargv values(?,?)"), &pssbuf, NULL);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        return RETVALFAILURE;
    }
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        goto failFinalize;
    }
    retintbuf = sqlite3_bind_blob(pssbuf, 2, pucBuf24, len, SQLITE_TRANSIENT);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        goto failFinalize;
    }
    retintbuf = sqlite3_step(pssbuf);
    if (retintbuf != SQLITE_DONE) {
        LOGERRs
        goto failFinalize;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(char){RETVALSUCCESS}, 1);
    if(ISRETVALERR){
        LOGERRs
        return RETVALFAILURE;
    }
    return RETVALSUCCESS;
    failFinalize:
    SLFINALIZEs(pssbuf)
    return RETVALFAILURE;
}

static int tempSpSelectEntailmentSets(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select v,d from in_entailment join in_singleargv on c=v where a=?"), &pssbuf, NULL);
#define REUSEBLOCKs                                                                 \
    if (retintbuf != SQLITE_OK) {                                                   \
        LOGERRs                                                                     \
        return RETVALFAILURE;                                                       \
    }                                                                               \
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);                               \
    if (retintbuf != SQLITE_OK) {                                                   \
        LOGERRs                                                                     \
        goto failFinalize;                                                          \
    }                                                                               \
    while(1){                                                                       \
        retintbuf = sqlite3_step(pssbuf);                                           \
        if(retintbuf != SQLITE_ROW)                                                 \
            break;                                                                  \
        retintbuf = sendUntilBytes(&(char){1}, 1);                                  \
        if(ISRETVALERR){                                                            \
            LOGERRs                                                                 \
            goto failFinalize;                                                      \
        }                                                                           \
        sqlite3_int64 sid = sqlite3_column_int64(pssbuf, 0);                        \
        retintbuf = sendUntilBytes(&sid, sizeof sid);                               \
        if(ISRETVALERR){                                                            \
            LOGERRs                                                                 \
            goto failFinalize;                                                      \
        }                                                                           \
        int blen = sqlite3_column_bytes(pssbuf, 1);                                 \
        const char *cbuf = sqlite3_column_blob(pssbuf, 1);                          \
        retintbuf = sendLen24WithLenChk(cbuf, blen);                                \
        if(ISRETVALERR){                                                            \
            LOGERRs                                                                 \
            goto failFinalize;                                                      \
        }                                                                           \
    }                                                                               \
    if (retintbuf != SQLITE_DONE){                                                  \
        LOGERRs                                                                     \
        goto failFinalize;                                                          \
    }                                                                               \
    SLFINALIZEs(pssbuf)                                                             \
    retintbuf = sendUntilBytes(&(char){0}, 1);                                      \
    if(ISRETVALERR){                                                                \
        LOGERRs                                                                     \
        return RETVALFAILURE;                                                       \
    }
    REUSEBLOCKs
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select v,d from in_entailment join in_singleargv on a=v where c=?"), &pssbuf, NULL);
    REUSEBLOCKs
#undef REUSEBLOCKs
    //? could this sql be better? (performance/bug-proneness):select * from in_mutexgroup where g in(select g from in_mutexgroup where v=?) and v<>?
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select a_a.g,a_a.v,d from in_singleargv join in_mutexgroup a_a on a_a.v=in_singleargv.v join in_mutexgroup a_b on a_a.g=a_b.g and a_a.v<>a_b.v where a_b.v=? order by a_a.g"), &pssbuf, NULL);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        return RETVALFAILURE;
    }
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        goto failFinalize;
    }
    sibuf = 0;//note 0 is special, cannot be used in group id
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        sqlite3_int64 sid = sqlite3_column_int64(pssbuf, 0);
        if(sid==sibuf){
            retintbuf = sendUntilBytes(&(char){1}, 1);
        }else{
            retintbuf = sendUntilBytes(&(char){2}, 1);
            sibuf=sid;
        }
        if(ISRETVALERR){
            LOGERRs
            goto failFinalize;
        }
        sid = sqlite3_column_int64(pssbuf, 1);
        retintbuf = sendUntilBytes(&sid, sizeof sid);
        if(ISRETVALERR){
            LOGERRs
            goto failFinalize;
        }
        int blen = sqlite3_column_bytes(pssbuf, 2);
        const char *cbuf = sqlite3_column_blob(pssbuf, 2);
        retintbuf = sendLen24WithLenChk(cbuf, blen);
        if(ISRETVALERR){
            LOGERRs
            goto failFinalize;
        }
    }
    if (retintbuf != SQLITE_DONE){
        LOGERRs
        goto failFinalize;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(char){0}, 1);
    if(ISRETVALERR){
        LOGERRs
        return RETVALFAILURE;
    }
    return RETVALSUCCESS;
    failFinalize:
    SLFINALIZEs(pssbuf)
    return RETVALFAILURE;
}


static int tempSpSelectSAProp(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select in_singleargv.v,d from in_singleargp join in_singleargv on in_singleargp.v=in_singleargv.v where in_singleargp.s=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        switch(retintbuf){
            case SQLITE_ROW:
                sibuf = sqlite3_column_int64(pssbuf, 0);
                retintbuf = SENDWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                uint_least32_t blen = sqlite3_column_bytes(pssbuf, 1);
                SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                continue;
            case SQLITE_DONE:
                SLFINALIZEs(pssbuf)
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
                IF_ISRETVALERR_LOGNRETURNs
                return RETVALSUCCESS;
            default:
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
        }
    }
}

static int tempSpInsertFileAllocName(void){
    int retintbuf;
    unsigned char snmbuf[MAXFILENAMELEN];
    uint_least32_t bloblen;
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select snm from sp_filestorag order by length(snm) desc, snm desc limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    switch(retintbuf){
        case SQLITE_DONE:
            bloblen = 1;
            snmbuf[0] = '0';
            break;
        case SQLITE_ROW:
            bloblen = sqlite3_column_bytes(pssbuf, 0);
            memcpy(snmbuf, sqlite3_column_blob(pssbuf, 0), bloblen);
            lcaseAlphanumIncrement(snmbuf, &bloblen);
            break;
        default:
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    sqlite3_int64 sibuf;
    retintbuf = retAvailGlobalId(&sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_filestorag values(?,?,?,?,?)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    void *blobbuf = snmbuf;
    uint_least32_t snmlen = bloblen;
    int bindInd = 2;
    beginbindblob:
    retintbuf = sqlite3_bind_blob(pssbuf, bindInd, blobbuf, bloblen, SQLITE_TRANSIENT);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    if(bindInd!=5){
        retintbuf = recvWithUcBuf24(&bloblen);
        if(ISRETVALERR){
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
        }
        blobbuf = pucBuf24;
        bindInd++;
        goto beginbindblob;
    }
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = SENDWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sendLen24(snmbuf, snmlen);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpInsertDataStoreInfo(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_datastinfo values(?,?)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    retintbuf = RECVWITHVAR(sibuf);
    if(ISRETVALERR){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibuf);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(char){0}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSearchAnimeWithFile(void){
    int retintbuf;
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    char chBuf;
    retintbuf = RECVWITHVAR(chBuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    if(chBuf){//note about the sql: not all essential text data are sent!!
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select a_a.i,mid,title,english,synonyms,episodes,type,t from(select i from sp_animationp union select i from sp_myanimelis)a_a left join sp_myanimelis on sp_myanimelis.i=a_a.i left join sp_essenttext on g=a_a.i where a_a.i in(select i from sp_datastinfo)and(t like ?1 escape ?2 or title like ?1 escape ?2 or english like ?1 escape ?2 or synonyms like ?1 escape ?2)order by a_a.i"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_text(pssbuf, 2, &chBuf, 1, SQLITE_TRANSIENT);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select a_a.i,mid,title,english,synonyms,episodes,type,t from(select i from sp_animationp union select i from sp_myanimelis)a_a left join sp_myanimelis on sp_myanimelis.i=a_a.i left join sp_essenttext on g=a_a.i where a_a.i in(select i from sp_datastinfo)and(t like ?1 or title like ?1 or english like ?1 or synonyms like ?1)order by a_a.i"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }
    retintbuf = sqlite3_bind_text(pssbuf, 1, (char *)pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    sqlite3_int64 sibuf;
    sqlite3_int64 lastid = 0;
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW){
            if(retintbuf != SQLITE_DONE){
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
            }
            if(lastid!=0){
                retintbuf = sendUntilBytes(SEVENEXPLICITNULLITERAL, 2);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
            }else{
                retintbuf = sendUntilBytes(SEVENEXPLICITNULLITERAL, 1);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
            }
            break;
        }
        int intbuf;
        sibuf = sqlite3_column_int64(pssbuf, 0);
        if(lastid == sibuf){
            intbuf = 7;
            goto retcol;
        }else if(lastid!=0){
            retintbuf = sendUntilBytes(&(char){0}, 1);
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
        }
        lastid = sibuf;
        retintbuf = sendUntilBytes(&(char){1}, 1);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        intbuf = 1;
        retcol:;
        int blen = sqlite3_column_bytes(pssbuf, intbuf);
        const char *cbuf = blen ? sqlite3_column_blob(pssbuf, intbuf) : NULL;
        if(intbuf!=7){
            SENDLEN24WITHLENCHKs(cbuf, blen)
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
            intbuf++;
            goto retcol;
        }else{
            if(blen){
                retintbuf = sendUntilBytes(&(char){1}, 1);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                sendLen24(cbuf, blen);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
            }
        }
    }
    SLFINALIZEs(pssbuf)
    return RETVALSUCCESS;
}

static int tempSpSelectAnime(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from sp_myanimelis where i=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 2);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 2),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 3);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 3),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 4);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 4),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 5);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 5),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 6);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 6),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }else if(retintbuf == SQLITE_DONE){
        retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, 18);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }else{
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select t from sp_essenttext where g=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        retintbuf = sendUntilBytes(&(char){1}, 1);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 0);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 0),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if(retintbuf != SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(char){0}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from sp_animationp where i=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 2);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 2),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 3);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 3),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }else if(retintbuf == SQLITE_DONE){
        retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, 9);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }else{
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select d,snm,size,wtime,onm from sp_datastinfo join sp_filestorag on d=sp_filestorag.i where sp_datastinfo.i=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        retintbuf = sendUntilBytes(&(char){1}, 1);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 2);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 2),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 3);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 3),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 4);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 4),blen)
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(char){0}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpChkAnimeRegistration(void){
    int retintbuf;
    
    uint_least32_t lenid;
    retintbuf=recvUnsigned24(&lenid);
    IF_ISRETVALERR_LOGNRETURNs
    char *strid=0;
    if(lenid){
        strid=malloc(lenid);
        if(!strid)return RETVALMEMALLOCFAIL;
        retintbuf=recvUntilBytes(strid, lenid);
        if(ISRETVALERR){LOGERRs free(strid); return retintbuf;}
    }
    uint_least32_t lentitle;
    retintbuf=recvUnsigned24(&lentitle);
    if(ISRETVALERR){LOGERRs free(strid); return retintbuf;}
    char *strtitle=0;
    if(lentitle){
        strtitle=malloc(lentitle);
        if(!strtitle){free(strid); return RETVALMEMALLOCFAIL;}
        retintbuf=recvUntilBytes(strtitle, lentitle);
        if(ISRETVALERR){LOGERRs free(strid);free(strtitle); return retintbuf;}
    }
    uint_least32_t leneng;
    retintbuf=recvUnsigned24(&leneng);
    if(ISRETVALERR){LOGERRs free(strid);free(strtitle); return retintbuf;}
    char *streng=0;
    if(leneng){
        streng=malloc(leneng);
        if(!streng){free(strid);free(strtitle); return RETVALMEMALLOCFAIL;}
        retintbuf=recvUntilBytes(streng, leneng);
        if(ISRETVALERR){LOGERRs free(strid);free(strtitle);free(streng); return retintbuf;}
    }
    
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select i from sp_myanimelis where mid=? and title=? and english=?"), &pssbuf, NULL);
    if(retintbuf != SQLITE_OK){LOGERRs free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
    retintbuf = slBindBlob(pssbuf, 1, strid, lenid, SQLITE_TRANSIENT);
    if(retintbuf != SQLITE_OK){LOGERRs SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
    retintbuf = slBindBlob(pssbuf, 2, strtitle, lentitle, SQLITE_TRANSIENT);
    if(retintbuf != SQLITE_OK){LOGERRs SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
    retintbuf = slBindBlob(pssbuf, 3, streng, leneng, SQLITE_TRANSIENT);
    if(retintbuf != SQLITE_OK){LOGERRs SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
    retintbuf = sqlite3_step(pssbuf);
    switch(retintbuf){
        case SQLITE_ROW:;
            sqlite3_int64 sibuf = sqlite3_column_int64(pssbuf, 0);
            SLFINALIZEs(pssbuf)
            free(strid);free(strtitle);free(streng);
            retintbuf = sendUntilBytes(&(unsigned char){254}, 1);
            IF_ISRETVALERR_LOGNRETURNs
            retintbuf = SENDWITHVAR(sibuf);
            IF_ISRETVALERR_LOGNRETURNs
            return RETVALSUCCESS;
        case SQLITE_DONE:
            break;
        default:
            LOGERRs SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    
    if(leneng){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from sp_myanimelis where mid=? or ? in(title,english)or ? in(title,english)"), &pssbuf, NULL);
        if(retintbuf != SQLITE_OK){LOGERRs free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
        retintbuf = slBindBlob(pssbuf, 3, streng, leneng, SQLITE_TRANSIENT);
        if(retintbuf != SQLITE_OK){LOGERRs SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from sp_myanimelis where mid=? or ? in(title,english)"), &pssbuf, NULL);
        if(retintbuf != SQLITE_OK){LOGERRs free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
    }
    retintbuf = slBindBlob(pssbuf, 1, strid, lenid, SQLITE_TRANSIENT);
    if(retintbuf != SQLITE_OK){LOGERRs SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
    retintbuf = slBindBlob(pssbuf, 2, strtitle, lentitle, SQLITE_TRANSIENT);
    if(retintbuf != SQLITE_OK){LOGERRs SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);return RETVALFAILURE;}
    retintbuf = sqlite3_step(pssbuf);
    switch(retintbuf){
        case SQLITE_ROW:
            SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);
            retintbuf = sendUntilBytes(&(unsigned char){253}, 1);
            IF_ISRETVALERR_LOGNRETURNs
            return RETVALSUCCESS;
        case SQLITE_DONE:
            SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);
            retintbuf = sendUntilBytes(&(unsigned char){1}, 1);
            IF_ISRETVALERR_LOGNRETURNs
            return RETVALSUCCESS;
        default:
            LOGERRs SLFINALIZEs(pssbuf) free(strid);free(strtitle);free(streng);return RETVALFAILURE;
    }
}

static int tempSpUpsertMalAnime(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    if(sibuf){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("update sp_myanimelis set mid=?2,title=?3,english=?4,synonyms=?5,episodes=?6,type=?7 where i=?1"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }else{
        retintbuf = retAvailGlobalId(&sibuf);
        IF_ISRETVALERR_LOGNRETURNs
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_myanimelis values(?,?,?,?,?,?,?)"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    uint_least32_t len;
    for(int intbuf = 2;;intbuf++){
        retintbuf = recvWithUcBuf24(&len);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_bind_blob(pssbuf, intbuf, pucBuf24, len, SQLITE_TRANSIENT);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        if(intbuf==7)
            break;
    }
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = SENDWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

//note related single-arg verbs are not deleted
static int tempSpDeleteFromFileStNDataStInfo(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from sp_datastinfo where d=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from sp_filestorag where i=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(char){0}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpDeleteShortNote(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_singleargp where s=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf != SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from sp_shortnotes where s=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf != SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(char){0}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSelectShortNote(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select t,in_singleargv.v,d from sp_shortnotes left join in_singleargp on sp_shortnotes.s=in_singleargp.s left join in_singleargv on in_singleargp.v=in_singleargv.v where sp_shortnotes.s=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    //undone if(retintbuf == SQLITE_DONE){
    //should add handling here for already deleted short note
    //}
    if(retintbuf != SQLITE_ROW){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    int blen = sqlite3_column_bytes(pssbuf, 0);
    SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 0), blen);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    while(1){
        retintbuf=sqlite3_column_type(pssbuf,1);
        switch(retintbuf){
            case SQLITE_NULL:
                break;
            case SQLITE_INTEGER:
                sibuf = sqlite3_column_int64(pssbuf, 1);
                retintbuf = SENDWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                blen = sqlite3_column_bytes(pssbuf, 2);
                SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 2), blen);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                break;
            default:
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
        }
        retintbuf = sqlite3_step(pssbuf);
        switch(retintbuf){
            case SQLITE_ROW:
                continue;
            case SQLITE_DONE:
                SLFINALIZEs(pssbuf)
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
                IF_ISRETVALERR_LOGNRETURNs
                return RETVALSUCCESS;
            default:
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
        }
    }
}

static int tempSpSearchShortNoteText(void){
    int retintbuf;
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    char chBuf;
    retintbuf = RECVWITHVAR(chBuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    if(chBuf){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select sp_shortnotes.s,t,in_singleargv.v,d from sp_shortnotes left join in_singleargp on sp_shortnotes.s=in_singleargp.s left join in_singleargv on in_singleargp.v=in_singleargv.v where t like ? escape ? order by sp_shortnotes.s"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_text(pssbuf, 2, &chBuf, 1, SQLITE_TRANSIENT);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select sp_shortnotes.s,t,in_singleargv.v,d from sp_shortnotes left join in_singleargp on sp_shortnotes.s=in_singleargp.s left join in_singleargv on in_singleargp.v=in_singleargv.v where t like ? order by sp_shortnotes.s"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }
    retintbuf = sqlite3_bind_text(pssbuf, 1, (char *)pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    sqlite3_int64 sibuf;
    sqlite3_int64 lastid = 0;
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW){
            if(retintbuf != SQLITE_DONE){
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
            }
            if(lastid!=0){
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64)*2);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
            }else{
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
            }
            break;
        }
        sibuf = sqlite3_column_int64(pssbuf, 0);
        if(lastid == sibuf){
            goto startOfRetVerb;
        }else if(lastid!=0){
            retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
        }
        lastid = sibuf;
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        if(blen){
            const char *cv = sqlite3_column_blob(pssbuf, 1);
            char *nln = memchr(cv, '\n', blen);
            char *nlr = memchr(cv, '\r', blen);
            if(nln){
                if(nlr)
                    blen = MINOFTWO(nln,nlr)-cv;
                else
                    blen = nln-cv;
                SENDLEN24WITHLENCHKs(cv, blen);
            }else if(nlr){
                blen = nlr-cv;
                SENDLEN24WITHLENCHKs(cv, blen);
            }else
                retintbuf = sendLen24(cv, blen);
        }else
            retintbuf = sendUntilBytes(SEVENEXPLICITNULLITERAL, 3);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        startOfRetVerb:
        retintbuf=sqlite3_column_type(pssbuf,2);
        switch(retintbuf){
            case SQLITE_NULL:
                break;
            case SQLITE_INTEGER:
                sibuf = sqlite3_column_int64(pssbuf, 2);
                retintbuf = SENDWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                blen = sqlite3_column_bytes(pssbuf, 3);
                SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 3), blen);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                break;
            default:
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
        }
    }
    SLFINALIZEs(pssbuf)
    return RETVALSUCCESS;
}
static int tempSpSearchShortNoteRelatedVerbs(void){//undone
    
    //search for all verbs that is not entailed by "is-a short note" && whose negation is not entailed by "is-a short note"
    //in other words, the props that are determined by "is-a short note" are not needed for show.
    //
    //only accept single-arg verbs
    
    
    //with aa2 as(select a from formulaorconj join formulaorform on p=r where f in (select f from formulaorform aa0 where p in (select r from formulaorconj where a=IS_A_SHORTNOTE_ARG)and(select count(*)from formulaorform aa1 where aa0.f=aa1.f)=2))select * from argvermapping where v not in(select v from argvermapping where a in aa2 and b in aa2)
    //select bytessensemap.b from TABNAME_OF_AN_ARG_ID_TO_BE_EXPLAINED_BY_UTF8_NATURAL_LANGUAGE_TEXT join bytessensemap on bytessensemap.a=UTF8TEXT where ARG_ID=?
    //
    return RETVALFAILURE;
}
static int tempSpSearchAnimeRelatedText(void){
    int retintbuf;
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    if(ISRETVALERR){
        LOGERRs
        //undone send (char)1 to client for failure code?
        return RETVALFAILURE;
    }
    char chBuf;
    sqlite3_stmt *pssbuf;
    retintbuf = recvUntilBytes(&chBuf, 1);
    if(ISRETVALERR){
        LOGERRs
        return RETVALFAILURE;
    }
    if(chBuf){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from sp_myanimelis where title like ?1 escape ?2 or english like ?1 escape ?2 or synonyms like ?1 escape ?2"), &pssbuf, NULL);
        if (retintbuf != SQLITE_OK) {
            LOGERRs
            goto fail;
        }
        retintbuf = sqlite3_bind_text(pssbuf, 2, &chBuf, 1, SQLITE_TRANSIENT);
        if (retintbuf != SQLITE_OK) {
            LOGERRs
            goto failFinalize;
        }
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from sp_myanimelis where title like ?1 or english like ?1 or synonyms like ?1"), &pssbuf, NULL);
        if (retintbuf != SQLITE_OK) {
            LOGERRs
            goto fail;
        }
    }
    retintbuf = sqlite3_bind_text(pssbuf, 1, (char *)pucBuf24, len, SQLITE_TRANSIENT);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        goto failFinalize;
    }
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        retintbuf = sendUntilBytes(&(char){1}, 1);
        if(ISRETVALERR){
            LOGERRs
            goto failFinalize;
        }
        sqlite3_int64 sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        if(ISRETVALERR){
            LOGERRs
            goto failFinalize;
        }
        for(int colInd = 1; colInd<7;colInd++){
            int blen = sqlite3_column_bytes(pssbuf, colInd);
            const char *cbuf = sqlite3_column_blob(pssbuf, colInd);
            retintbuf = sendLen24WithLenChk(cbuf, blen);
            if(ISRETVALERR){
                LOGERRs
                goto failFinalize;
            }
        }
    }
    if (retintbuf != SQLITE_DONE){
        LOGERRs
        goto failFinalize;
    }
    retintbuf = sendUntilBytes(&(char){0}, 1);
    if(ISRETVALERR){
        LOGERRs
        goto failFinalize;
    }
    SLFINALIZEs(pssbuf)
            
            
    if(chBuf){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select g,t,mid,episodes,type from sp_essenttext left join sp_myanimelis on i=g where g in(select i from sp_animationp union all select i from sp_myanimelis) and t like ? escape ?"), &pssbuf, NULL);
        if (retintbuf != SQLITE_OK) {
            LOGERRs
            goto fail;
        }
        retintbuf = sqlite3_bind_text(pssbuf, 2, &chBuf, 1, SQLITE_TRANSIENT);
        if (retintbuf != SQLITE_OK) {
            LOGERRs
            goto failFinalize;
        }
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select g,t,mid,episodes,type from sp_essenttext left join sp_myanimelis on i=g where g in(select i from sp_animationp union all select i from sp_myanimelis) and t like ?"), &pssbuf, NULL);
        if (retintbuf != SQLITE_OK) {
            LOGERRs
            goto fail;
        }
    }
    retintbuf = sqlite3_bind_text(pssbuf, 1, (char *)pucBuf24, len, SQLITE_TRANSIENT);
    if (retintbuf != SQLITE_OK) {
        LOGERRs
        goto failFinalize;
    }
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        retintbuf = sendUntilBytes(&(char){1}, 1);
        if(ISRETVALERR){
            LOGERRs
            goto failFinalize;
        }
        sqlite3_int64 sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        if(ISRETVALERR){
            LOGERRs
            goto failFinalize;
        }
        for(int colInd = 1; colInd<5;colInd++){
            int blen = sqlite3_column_bytes(pssbuf, colInd);
            const char *cbuf = sqlite3_column_blob(pssbuf, colInd);
            retintbuf = sendLen24WithLenChk(cbuf, blen);
            if(ISRETVALERR){
                LOGERRs
                goto failFinalize;
            }
        }
    }
    if (retintbuf != SQLITE_DONE){
        LOGERRs
        goto failFinalize;
    }
    retintbuf = sendUntilBytes(&(char){0}, 1);
    if(ISRETVALERR){
        LOGERRs
        goto failFinalize;
    }
    SLFINALIZEs(pssbuf)
            
            
    return RETVALSUCCESS;
    failFinalize:
    SLFINALIZEs(pssbuf)
    fail:
    return RETVALFAILURE;
}

static int tempSpSelectShortNoteWithVerb(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    if(ISRETVALERR){
        LOGERRs
        //undone send (char)1 to client for failure code?
        return RETVALFAILURE;
    }
    
    
    //with a_a(z)as(values(?)union all select a from in_entailment join a_a on c=z)select * from sp_shortnotes where s in(select s from in_singleargp where v in a_a)
    return 0;
}

static int tempSpUpsertShortNoteTextOnly(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    sqlite3_stmt *pssbuf;
    if(sibuf){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("update sp_shortnotes set t=?1 where t<>?1 and s=?2"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_blob(pssbuf, 1, pucBuf24, len, SQLITE_TRANSIENT);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 2, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    }else{
        retintbuf = retAvailGlobalId(&sibuf);
        IF_ISRETVALERR_LOGNRETURNs
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_shortnotes values(?,?)"), &pssbuf, NULL);
        IF_ISRETVALERR_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_bind_blob(pssbuf, 2, pucBuf24, len, SQLITE_TRANSIENT);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    }
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = SENDWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpDeleteSAVerb(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from in_mutexgroup where v=?1 union all select 0 from in_entailment where c=?1 union all select 0 from in_singleargp where v=?1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALFAILURE}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_entailment where a=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_singleargv where v=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempReceiveSingleArgPropsWithTmpTabAndChk(void){
    int retintbuf;
    sqlite3_stmt *pssbuf;
    retintbuf = readyEmptyTmpTabI0();
    if(ISRETVALERR) return RETVALFAILURE;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into tmptab_i0 values(?)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    sqlite3_int64 siver;
    do{
        retintbuf = RECVWITHVAR(siver);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, siver);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
        }
        char chBuf;
        retintbuf = recvUntilBytes(&chBuf, 1);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        if(!chBuf)
            break;
        sqlite3_reset(pssbuf);
    }while(1);
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from tmptab_i0 where z not in(select v from in_singleargv)limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALNOTFOUND}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALNOTFOUND;
    }
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from tmptab_i0 group by z having count(*)>1 limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALTAUTOLOGY}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALTAUTOLOGY;
    }
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    //note check oxymoron, if any, respond cannot save
    //note UNION instead of UNION ALL, coz you can have duplicates in a_a with UNION ALL, then you get false positives.
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_a(z)as(select z from tmptab_i0 union select c from in_entailment join a_a on a=z)select 0 from a_a join in_mutexgroup on v=z group by g having count(*)>1 limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALOXYMORON}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALOXYMORON;
    }
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    //note check tautology, if any, respond doubt
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_b(y)as(select c from in_entailment join tmptab_i0 on a=z union all select c from in_entailment join a_b on a=y)select 0 from tmptab_i0 join a_b on z=y limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALTAUTOLOGY}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALTAUTOLOGY;
    }
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    return RETVALSUCCESS;
}

static int tempSpSaveSAProps(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    char chBuf;
    retintbuf = recvUntilBytes(&chBuf, 1);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    if(chBuf){
        switch(tempReceiveSingleArgPropsWithTmpTabAndChk()){
            case RETVALSUCCESS:
                break;
            case RETVALNOTFOUND:
            case RETVALTAUTOLOGY:
            case RETVALOXYMORON:
                return RETVALSUCCESS;
            default:
                return retintbuf;
        }
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_singleargp where s=? and v not in tmptab_i0 and v<>" XSTR(IS_A_NATURAL_LANG_WORD_SENSE)), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
        }
        SLFINALIZEs(pssbuf)
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_singleargp select z,?1 from tmptab_i0 where z not in(select v from in_singleargp where s=?1)"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
        }
        SLFINALIZEs(pssbuf)
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_singleargp where s=? and v<>" XSTR(IS_A_NATURAL_LANG_WORD_SENSE)), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
        }
        SLFINALIZEs(pssbuf)
    }
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpUpsertShortNote(void){
    int retintbuf;
    uint_least32_t len;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    char chBuf;
    retintbuf = recvUntilBytes(&chBuf, 1);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    //todo check note id existing? (might have been delete by some client)
    //todo check text blob duplicate existing when new note is about to insert?
    if(chBuf){
        switch(tempReceiveSingleArgPropsWithTmpTabAndChk()){
            case RETVALSUCCESS:
                break;
            case RETVALTAUTOLOGY:
            case RETVALOXYMORON:
                return RETVALSUCCESS;
            default:
                return retintbuf;
        }
        if(sibuf){
            retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_singleargp where s=? and v not in tmptab_i0"), &pssbuf, NULL);
            IFNOT_SQLITE_OK_LOGNRETURNs
            retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_step(pssbuf);
            if(retintbuf!=SQLITE_DONE){
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
            }
            SLFINALIZEs(pssbuf)
            retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_singleargp select z,?1 from tmptab_i0 where z not in(select v from in_singleargp where s=?1)"), &pssbuf, NULL);
            IFNOT_SQLITE_OK_LOGNRETURNs
            retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_step(pssbuf);
            if(retintbuf!=SQLITE_DONE){
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
            }
            SLFINALIZEs(pssbuf)
            goto updateOldNote;
        }else{
            retintbuf = retAvailGlobalId(&sibuf);
            IF_ISRETVALERR_LOGNRETURNs
            retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_singleargp select z,? from tmptab_i0"), &pssbuf, NULL);
            IFNOT_SQLITE_OK_LOGNRETURNs
            retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_step(pssbuf);
            if(retintbuf!=SQLITE_DONE){
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
            }
            SLFINALIZEs(pssbuf)
            goto insertNewNote;
        }
    }else{
        if(sibuf){
            retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_singleargp where s=?"), &pssbuf, NULL);
            IFNOT_SQLITE_OK_LOGNRETURNs
            retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_step(pssbuf);
            if(retintbuf!=SQLITE_DONE){
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
            }
            SLFINALIZEs(pssbuf)
            goto updateOldNote;
        }
        retintbuf = retAvailGlobalId(&sibuf);
        IF_ISRETVALERR_LOGNRETURNs
    }
    
    insertNewNote:
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_shortnotes (?,?)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_blob(pssbuf, 2, pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = SENDWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
    
    updateOldNote:
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    //? better sql?: "update sp_shortnotes set t=?1 where t<>?1 and s=(select s from sp_shortnotes where s=?2 limit 1)"
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("update sp_shortnotes set t=?1 where t<>?1 and s=?2"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_blob(pssbuf, 1, pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;

//        MEMCPYLITERALnADVANCEs(pcbuf, "with a_a(z)as(values(?)")
//        for(uint_least32_t i=1;i<len;i++){
//            MEMCPYLITERALnADVANCEs(pcbuf, ",(?)")
//        }
//        pcsav = pcbuf;
//        MEMCPYLITERALNULnADVANCEs(pcbuf,"union select c from in_entailment join a_a on a=z)select 0 from a_a join in_mutexgroup on v=z group by g having count(z)>1 limit 1")
//        MEMCPYLITERALNULnADVANCEs(pcsav,"),a_b(y)as(select c from in_entailment join a_a on a=z union all select c from in_entailment join a_b on a=y)select 0 from a_a join a_b on z=y limit 1")

}


static int tempSpChkPropsValidity(void){
    int retintbuf;
    char chBuf;
    retintbuf = recvUntilBytes(&chBuf, 1);
    IF_ISRETVALERR_LOGNRETURNs
    if(chBuf){
        switch(tempReceiveSingleArgPropsWithTmpTabAndChk()){
            case RETVALSUCCESS:
                break;
            case RETVALNOTFOUND:
            case RETVALTAUTOLOGY:
            case RETVALOXYMORON:
                return RETVALSUCCESS;
            default:
                return retintbuf;
        }
    }
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

//note client side needs to re-request this function each time a verb is applied. Applying multiple verbs simultaneously is NO GOOD!
//coz this function tells you who are whose parent, and mutually exclusive verbs are immediately removed from user options!
//note this function should be useful whenever you are preparing a set of verbs:
//1. maybe you are preparing a set of verbs to apply to a sense
//2. maybe you are preparing a set of verbs for building an advanced search criteria
static int tempSpApplicableGeneralVerbs(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    char chBuf;
    retintbuf = recvUntilBytes(&chBuf, 1);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    if(chBuf){
        switch(tempReceiveSingleArgPropsWithTmpTabAndChk()){
            case RETVALSUCCESS:
                break;
            case RETVALNOTFOUND:
            case RETVALTAUTOLOGY:
            case RETVALOXYMORON:
                return RETVALSUCCESS;
            default:
                return retintbuf;
        }
        retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        //1.exclude entailed verbs 2.exclude verbs that can entail applicable verbs 3.exclude verbs in mutex groups (no need to exclude verbs that entail verbs of <3> because <2> already excludes them) 4.exclude verbs that entail applied verbs (no need to exclude grandchildren because <2> already excludes them)
        //note union all is probably better than union coz duplicate parents won't be very common for real world models
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_a(z)as(select z from tmptab_i0 union all select c from in_entailment join a_a on a=z)select * from in_singleargv where v not in a_a and v not in(select a from in_entailment where c not in a_a) and v not in(select v from in_mutexgroup where g in(select g from in_mutexgroup where v in a_a)) and v not in(select a from in_entailment where c in tmptab_i0)"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }else{
        retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from in_singleargv where v not in(select a from in_entailment)"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        switch(retintbuf){
            case SQLITE_ROW:
                sibuf = sqlite3_column_int64(pssbuf, 0);
                retintbuf = SENDWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                uint_least32_t blen = sqlite3_column_bytes(pssbuf, 1);
                SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                continue;
            case SQLITE_DONE:
                SLFINALIZEs(pssbuf)
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
                IF_ISRETVALERR_LOGNRETURNs
                return RETVALSUCCESS;
            default:
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
        }
    }
}

static int tempSpUpsertAnimation(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from sp_animationp where i=? limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf==SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("update sp_animationp set prog=?2,rank=?3,comm=?4 where i=?1"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }else if(retintbuf==SQLITE_DONE){
        SLFINALIZEs(pssbuf)
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_animationp values(?,?,?,?)"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }else{
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_blob(pssbuf, 2, pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_blob(pssbuf, 3, pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_blob(pssbuf, 4, pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

//note related single-arg verbs are not deleted
static int tempSpDeleteAnimation(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from sp_animationp where i=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpUpsertSAVerb(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select v from in_singleargv where d=? limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_blob(pssbuf, 1, pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    switch(retintbuf){
        case SQLITE_ROW:
            retintbuf = sendUntilBytes(&(unsigned char){sibuf==sqlite3_column_int64(pssbuf, 0)?RETVALSUCCESS:RETVALFAILURE}, 1);
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
            SLFINALIZEs(pssbuf)
            return RETVALSUCCESS;
        case SQLITE_DONE:
            SLFINALIZEs(pssbuf)
            if(sibuf){
                retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("update in_singleargv set d=? where v=?"), &pssbuf, NULL);
                IFNOT_SQLITE_OK_LOGNRETURNs
                retintbuf = sqlite3_bind_blob(pssbuf, 1, pucBuf24, len, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                retintbuf = sqlite3_bind_int64(pssbuf, 2, sibuf);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                retintbuf = sqlite3_step(pssbuf);
                if(retintbuf!=SQLITE_DONE){
                    LOGERRs
                    SLFINALIZEs(pssbuf)
                    return RETVALFAILURE;
                }
                SLFINALIZEs(pssbuf)
                retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
                IF_ISRETVALERR_LOGNRETURNs
            }else{
                retintbuf = retAvailGlobalId(&sibuf);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_singleargv values(?,?)"), &pssbuf, NULL);
                IFNOT_SQLITE_OK_LOGNRETURNs
                retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                retintbuf = sqlite3_bind_blob(pssbuf, 2, pucBuf24, len, SQLITE_TRANSIENT);
                IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                retintbuf = sqlite3_step(pssbuf);
                if(retintbuf!=SQLITE_DONE){
                    LOGERRs
                    SLFINALIZEs(pssbuf)
                    return RETVALFAILURE;
                }
                SLFINALIZEs(pssbuf)
                retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = SENDWITHVAR(sibuf);
                IF_ISRETVALERR_LOGNRETURNs
            }
            return RETVALSUCCESS;
        default:
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
    }
}

static int tempSpSelectSAVerb(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select d from in_singleargv where v=? limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    switch(retintbuf){
        case SQLITE_ROW:;
            uint_least32_t blen = sqlite3_column_bytes(pssbuf, 0);
            SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 0), blen);
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
            SLFINALIZEs(pssbuf)
            return RETVALSUCCESS;
        case SQLITE_DONE:
            SLFINALIZEs(pssbuf)
            retintbuf = sendUntilBytes(SEVENEXPLICITNULLITERAL, 3);
            IF_ISRETVALERR_LOGNRETURNs
            return RETVALSUCCESS;
        default:
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
    }
}

static int tempSpSearchWordSenseGroup(void){
    int retintbuf;
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    char chBuf;
    retintbuf = RECVWITHVAR(chBuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    if(chBuf){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select a_a.g,a_a.t,a_b.t from sp_essenttext a_a left join sp_essenttext a_b on a_a.g=a_b.g and a_a.t<>a_b.t where a_a.g in (select s from in_singleargp where v=" XSTR(IS_A_NATURAL_LANG_WORD_SENSE) ") and a_a.t like ? escape ? order by a_a.g,a_a.t"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_text(pssbuf, 2, &chBuf, 1, SQLITE_TRANSIENT);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select a_a.g,a_a.t,a_b.t from sp_essenttext a_a left join sp_essenttext a_b on a_a.g=a_b.g and a_a.t<>a_b.t where a_a.g in (select s from in_singleargp where v=" XSTR(IS_A_NATURAL_LANG_WORD_SENSE) ") and a_a.t like ? order by a_a.g,a_a.t"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }
    retintbuf = sqlite3_bind_text(pssbuf, 1, (char *)pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    sqlite3_int64 sibuf;
#define LASTLEN globalint0
    //int lastLen=0;//it's safe to NOT initialize lastLen as long as sibuf never equals 0
    sqlite3_int64 lastid = 0;
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW){
            if(retintbuf != SQLITE_DONE){
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
            }
            if(lastid!=0){
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, 1+sizeof(sqlite3_int64));
            }else{
                retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
            }
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
            break;
        }
        sibuf = sqlite3_column_int64(pssbuf, 0);
        int blen = sqlite3_column_bytes(pssbuf, 1);
        const char *cv = sqlite3_column_blob(pssbuf, 1);
        if(lastid == sibuf && LASTLEN == blen && MEMCMPifLEN(pucBuf24, cv, blen)==0){
            goto startOfRetOtherMembers;
        }else if(lastid!=0){
            retintbuf = sendUntilBytes(&(char){0}, 1);
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
        }
        lastid = sibuf; LASTLEN = blen; MEMCPYifLEN(pucBuf24, cv, blen);
#undef LASTLEN
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        SENDLEN24WITHLENCHKs(cv, blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        startOfRetOtherMembers:
        retintbuf=sqlite3_column_type(pssbuf,2);
        switch(retintbuf){
            case SQLITE_NULL:
                break;
            case SQLITE_BLOB:
                retintbuf = sendUntilBytes(&(char){1}, 1);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                blen = sqlite3_column_bytes(pssbuf, 2);
                SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 2), blen);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                break;
            default:
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
        }
    }
    SLFINALIZEs(pssbuf)
    return RETVALSUCCESS;
}

static int tempSpUpsertOrDeleteWordSenseGroup(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    char chBuf;
    retintbuf = recvUntilBytes(&chBuf, 1);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    if(chBuf){
        retintbuf = readyEmptyTmpTabB0();
        if(ISRETVALERR) return RETVALFAILURE;
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into tmptab_b0 values(?)"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        do{
            uint_least32_t len;
            retintbuf = recvWithUcBuf24(&len);
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_bind_blob(pssbuf, 1, pucBuf24, len, SQLITE_TRANSIENT);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_step(pssbuf);
            if(retintbuf!=SQLITE_DONE){
                LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;
            }
            retintbuf = recvUntilBytes(&chBuf, 1);
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
            if(!chBuf)
                break;
            sqlite3_reset(pssbuf);
        }while(1);
        SLFINALIZEs(pssbuf)
        if(sibuf){
            retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from sp_essenttext where g=? and t not in tmptab_b0"), &pssbuf, NULL);
            IFNOT_SQLITE_OK_LOGNRETURNs
            retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_step(pssbuf);
            if(retintbuf!=SQLITE_DONE){
                LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;
            }
            SLFINALIZEs(pssbuf)
            retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_essenttext select ?1,z from tmptab_b0 where z not in(select t from sp_essenttext where g=?1)"), &pssbuf, NULL);
        }else{
            retintbuf = retAvailGlobalId(&sibuf);
            IF_ISRETVALERR_LOGNRETURNs
            retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_essenttext select ?,z from tmptab_b0"), &pssbuf, NULL);
            IFNOT_SQLITE_OK_LOGNRETURNs
            retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_step(pssbuf);
            if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
            SLFINALIZEs(pssbuf)
            retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_singleargp values(" XSTR(IS_A_NATURAL_LANG_WORD_SENSE) ",?)"), &pssbuf, NULL);
        }
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
        SLFINALIZEs(pssbuf)
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from sp_essenttext where g=?"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
        }
        SLFINALIZEs(pssbuf)
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_singleargp where s=?"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){
            LOGERRs
            SLFINALIZEs(pssbuf)
            return RETVALFAILURE;
        }
        SLFINALIZEs(pssbuf)
    }
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = SENDWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSelectEssentialText(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select t from sp_essenttext where g=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        switch(retintbuf){
            case SQLITE_ROW:
                retintbuf = sendUntilBytes(&(unsigned char){1}, 1);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                uint_least32_t blen = sqlite3_column_bytes(pssbuf, 0);
                SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 0), blen);
                IF_ISRETVALERR_LOGNFINALIZENRETURNs
                continue;
            case SQLITE_DONE:
                SLFINALIZEs(pssbuf)
                retintbuf = sendUntilBytes(&(unsigned char){0}, 1);
                IF_ISRETVALERR_LOGNRETURNs
                return RETVALSUCCESS;
            default:
                LOGERRs
                SLFINALIZEs(pssbuf)
                return RETVALFAILURE;
        }
    }
}

static int tempSpUpsertUrlBookmark(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select i,d from sp_urlbookmar where u=? limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_blob(pssbuf, 1, pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    switch(retintbuf){
        case SQLITE_ROW:
            retintbuf = recvWithUcBuf24(&len);//note must discard the third argument string by reading it even if it's useless
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
            if(sibuf==sqlite3_column_int64(pssbuf, 0)){
                uint_least32_t blen = sqlite3_column_bytes(pssbuf, 1);
                if(blen!=len || MEMCMPifLEN(sqlite3_column_blob(pssbuf, 1),pucBuf24,len)!=0){
                    SLFINALIZEs(pssbuf)
                    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("update sp_urlbookmar set d=? where i=?"), &pssbuf, NULL);
                    IFNOT_SQLITE_OK_LOGNRETURNs
                    retintbuf = sqlite3_bind_blob(pssbuf, 1, pucBuf24, len, SQLITE_TRANSIENT);
                    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibuf);
                    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
                    retintbuf = sqlite3_step(pssbuf);
                    if (retintbuf != SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
                }
                SLFINALIZEs(pssbuf)
                retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = SENDWITHVAR(sibuf);
            }else{
                SLFINALIZEs(pssbuf)
                retintbuf = sendUntilBytes(&(unsigned char){RETVALFAILURE}, 1);
            }
            IF_ISRETVALERR_LOGNRETURNs
            return RETVALSUCCESS;
        case SQLITE_DONE:
            SLFINALIZEs(pssbuf)
            if(sibuf){
                retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("update sp_urlbookmar set u=?2,d=?3 where i=?1"), &pssbuf, NULL);
            }else{
                retintbuf = retAvailGlobalId(&sibuf);
                IF_ISRETVALERR_LOGNRETURNs
                retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into sp_urlbookmar values(?,?,?)"), &pssbuf, NULL);
            }
            IFNOT_SQLITE_OK_LOGNRETURNs
            retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_bind_blob(pssbuf, 2, pucBuf24, len, SQLITE_TRANSIENT);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = recvWithUcBuf24(&len);
            IF_ISRETVALERR_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_bind_blob(pssbuf, 3, pucBuf24, len, SQLITE_TRANSIENT);
            IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
            retintbuf = sqlite3_step(pssbuf);
            if (retintbuf != SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
            SLFINALIZEs(pssbuf)
            //note I could've used sqlite3_changes() to check if it's 0 (it might have been deleted before updating). But unfortunately the release note of 3.8.11.1 (a newer version) says: Fix a long-standing problem in sqlite3_changes() for WITHOUT ROWID tables that was reported a few hours after the 3.8.11 release.
            retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
            IF_ISRETVALERR_LOGNRETURNs
            retintbuf = SENDWITHVAR(sibuf);
            IF_ISRETVALERR_LOGNRETURNs
            return RETVALSUCCESS;
        default:
            LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;
    }
}

static int tempSpDeleteUrlBookmark(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_singleargp where s=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if (retintbuf != SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from sp_urlbookmar where i=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if (retintbuf != SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSearchDescNUrlInUrlBookmark(void){
    int retintbuf;
    uint_least32_t len;
    retintbuf = recvWithUcBuf24(&len);
    IF_ISRETVALERR_LOGNRETURNs
    char chBuf;
    retintbuf = RECVWITHVAR(chBuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    if(chBuf){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from sp_urlbookmar where u like ?1 escape ?2 or d like ?1 escape ?2"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_text(pssbuf, 2, &chBuf, 1, SQLITE_TRANSIENT);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    }else{
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from sp_urlbookmar where u like ?1 or d like ?1"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
    }
    retintbuf = sqlite3_bind_text(pssbuf, 1, (char *)pucBuf24, len, SQLITE_TRANSIENT);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        sqlite3_int64 sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        blen = sqlite3_column_bytes(pssbuf, 2);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 2), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if (retintbuf != SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSelectUrlBookmark(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select u,d from sp_urlbookmar where i=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    //undone if(retintbuf == SQLITE_DONE){
    //should add handling here for already deleted short note
    //}
    if(retintbuf != SQLITE_ROW){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    int blen = sqlite3_column_bytes(pssbuf, 0);
    SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 0), blen);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    blen = sqlite3_column_bytes(pssbuf, 1);
    SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    SLFINALIZEs(pssbuf)
    return RETVALSUCCESS;
}

static int tempSpSelectVerbsDirectlyEntailedByThis(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select v,d from in_entailment join in_singleargv on c=v where a=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_ROW) break;
        sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSelectVerbsDirectlyEntailingThis(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select v,d from in_entailment join in_singleargv on a=v where c=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_ROW) break;
        sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpDeleteEntailment(void){
    int retintbuf;
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_entailment where a=? and c=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpInsertEntailment(void){
    int retintbuf;
    sqlite3_int64 sibufa,sibufc;
    retintbuf = RECVWITHVAR(sibufa);
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = RECVWITHVAR(sibufc);
    IF_ISRETVALERR_LOGNRETURNs
    //check must be different
    if(sibufa==sibufc){
        retintbuf = sendUntilBytes(&(unsigned char){RETVALINVALIDSTATE}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    sqlite3_stmt *pssbuf;
    //check existence of 2 verbs
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from in_singleargv where v=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibufa);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_ROW){
        if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALNOTFOUND}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    sqlite3_reset(pssbuf);
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibufc);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_ROW){
        if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALNOTFOUND}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    SLFINALIZEs(pssbuf)
    //tautology a/any child of a directly entails c/parents of c (once a verb is directly entailed by another verb, it should not be indirectly entailed by that verb. i.e. a->b->c+a->d->c is okay, but a->c+a->d->c is tautology)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_a(z)as(values(?)union select a from in_entailment join a_a on c=z),a_c(x)as(values(?)union select c from in_entailment join a_c on a=x)select * from in_entailment where a in a_a and c in a_c"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibufa);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibufc);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf==SQLITE_ROW){//todo send a and c (with description) to client
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALTAUTOLOGY}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    //tautology similar to above, a entails c already(directly or indirectly)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_a(z)as(values(?) union select c from in_entailment join a_a on a=z)select 0 from a_a where z=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibufa);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibufc);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf==SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALTAUTOLOGY}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    sqlite3_reset(pssbuf);
    //oxymoron circle entailment
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibufc);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibufa);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf==SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALOXYMORON}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    //oxymoron any child of a (or just a itself) is mutually exclusive with c
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_a(z)as(values(?)union select a from in_entailment join a_a on c=z),a_b(y)as(select z from a_a union select c from in_entailment join a_b on a=y),a_c(x)as(values(?)union select c from in_entailment join a_c on a=x)select * from (select * from in_mutexgroup where v in a_b)a_d join (select * from in_mutexgroup where v in a_c)a_e on a_d.g=a_e.g and a_d.v<>a_e.v"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibufa);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibufc);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf==SQLITE_ROW){//todo send g and v (with description) to client
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALOXYMORON}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    //INVALID STATE if a sense already has two verbs, these 2 verbs cannot entail each other
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_a(z)as(values(?)union select a from in_entailment join a_a on c=z),a_c(x)as(values(?)union select c from in_entailment join a_c on a=x)select * from(select * from in_singleargp where v in a_a)a_d join(select * from in_singleargp where v in a_c)a_e on a_d.s=a_e.s"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibufa);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibufc);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf==SQLITE_ROW){//todo send v and s (with description) to client
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALINVALIDSTATE}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    //start inserting
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_entailment values(?,?)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibufa);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 2, sibufc);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpDeleteMutex(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_mutexgroup where g=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpUpsertMutex(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    sqlite3_stmt *pssbuf;
    retintbuf = readyEmptyTmpTabI0();
    if(ISRETVALERR) return RETVALFAILURE;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into tmptab_i0 values(?)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    do{
        retintbuf = RECVWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
        char chBuf;
        retintbuf = recvUntilBytes(&chBuf, 1);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        if(!chBuf)
            break;
        sqlite3_reset(pssbuf);
    }while(1);
    SLFINALIZEs(pssbuf)
    retintbuf = RECVWITHVAR(sibuf);//still need to recv it even if it's not used
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from tmptab_i0 where z not in(select v from in_singleargv)limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALNOTFOUND}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select 0 from tmptab_i0 group by z having count(*)>1 limit 1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALTAUTOLOGY}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    //INVALID STATE a sense having mutually exclusive verbs
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_a(z)as(select z from tmptab_i0 union select a from in_entailment join a_a on c=z)select s from in_singleargp where v in a_a group by s having count(*)>1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALINVALIDSTATE}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    //oxymoron a verb entails another verb, or two verbs entailed by some verb, then the 2 verbs must not be mutually exclusive
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("with a_a(y,z)as(select z,z from tmptab_i0 union select y,a from in_entailment join a_a on c=z)select z from a_a group by z having count(*)>1"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALOXYMORON}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    
    //tautology really difficult? e.g. you have 3 groups a-b-e,b-c-f,a-c-g, this makes a-b-c excessive?
    //undone implement THOROUGH tautology checking!!!!!!!
    //so you only check superset and subset, no further checking is implemented
    //subset or same set
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from in_mutexgroup where g<>? and g not in(select g from in_mutexgroup where v not in tmptab_i0)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALTAUTOLOGY}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    //superset or same set
    //select * from in_mutexgroup where not exists(select * from tmptab_i0 where z not in (select v from in_mutexgroup where g=g)  select * from in_mutexgroup where v in tmptab_i0 group by 
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select g from in_mutexgroup where g<>? and v in tmptab_i0 group by g having count(*)=(select count(*) from tmptab_i0)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    retintbuf = sqlite3_step(pssbuf);
    if(retintbuf == SQLITE_ROW){
        SLFINALIZEs(pssbuf)
        retintbuf = sendUntilBytes(&(unsigned char){RETVALTAUTOLOGY}, 1);
        IF_ISRETVALERR_LOGNRETURNs
        return RETVALSUCCESS;
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    if(sibuf){
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("delete from in_mutexgroup where g=? and v not in tmptab_i0"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
        SLFINALIZEs(pssbuf)
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_mutexgroup select ?1,z from tmptab_i0 where z not in(select v from in_mutexgroup where g=?1)"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
        SLFINALIZEs(pssbuf)
    }else{
        retintbuf = retAvailGlobalId(&sibuf);
        IF_ISRETVALERR_LOGNRETURNs
        retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("insert into in_mutexgroup select ?,z from tmptab_i0"), &pssbuf, NULL);
        IFNOT_SQLITE_OK_LOGNRETURNs
        retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
        IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
        SLFINALIZEs(pssbuf)
    }
    retintbuf = sendUntilBytes(&(unsigned char){RETVALSUCCESS}, 1);
    IF_ISRETVALERR_LOGNRETURNs
    retintbuf = SENDWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSelectMutexContainingVerb(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select g,d from in_mutexgroup join in_singleargv on in_singleargv.v=in_mutexgroup.v where in_mutexgroup.v<>?1 and g in(select g from in_mutexgroup where v=?1) order by g"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_ROW) break;
        sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSelectMutexGroup(void){
    int retintbuf;
    sqlite3_int64 sibuf;
    retintbuf = RECVWITHVAR(sibuf);
    IF_ISRETVALERR_LOGNRETURNs
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select in_mutexgroup.v,d from in_mutexgroup join in_singleargv on in_singleargv.v=in_mutexgroup.v where g=?"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    retintbuf = sqlite3_bind_int64(pssbuf, 1, sibuf);
    IFNOT_SQLITE_OK_LOGNFINALIZENRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf!=SQLITE_ROW) break;
        sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if(retintbuf!=SQLITE_DONE){LOGERRs SLFINALIZEs(pssbuf) return RETVALFAILURE;}
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

static int tempSpSelectVerbsThatDoNotEntail(void){
    int retintbuf;
    sqlite3_stmt *pssbuf;
    retintbuf = sqlite3_prepare_v2(databaseA, LITERALcommaSIZE("select * from in_singleargv where v not in(select a from in_entailment)"), &pssbuf, NULL);
    IFNOT_SQLITE_OK_LOGNRETURNs
    while(1){
        retintbuf = sqlite3_step(pssbuf);
        if(retintbuf != SQLITE_ROW)
            break;
        sqlite3_int64 sibuf = sqlite3_column_int64(pssbuf, 0);
        retintbuf = SENDWITHVAR(sibuf);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
        int blen = sqlite3_column_bytes(pssbuf, 1);
        SENDLEN24WITHLENCHKs(sqlite3_column_blob(pssbuf, 1), blen);
        IF_ISRETVALERR_LOGNFINALIZENRETURNs
    }
    if (retintbuf != SQLITE_DONE){
        LOGERRs
        SLFINALIZEs(pssbuf)
        return RETVALFAILURE;
    }
    SLFINALIZEs(pssbuf)
    retintbuf = sendUntilBytes(THIRTYONEEXPLICITNULLITERAL, sizeof(sqlite3_int64));
    IF_ISRETVALERR_LOGNRETURNs
    return RETVALSUCCESS;
}

//undone priority order
//short note
//word sense group
//url bookmarks with single-arg verbs
//sp_mtextficti,sp_audioprodu,sp_compileemb
//sp_personifie, sp_authorinfo,sp_translinfo
//auth info (for security reasons let's not do this for now)


//undone
//for the future of your gui technology stack, one problem is automation. You need to make sure you have a manageable scripting feature to allow automating any gui event dispatching.


//undone
//1. 1st we implement the plain tag - short notes, anime, files
// including creating plain verbs, assigning verbs to senses, changing verb desc
//2. then we start to implement verb entailment
// 2.1 this includes navigating and editing entailment tree
// 2.2 navigating and editing mutex groups
// 2.3 prompt applicable verbs when you do the EDITING above, or APPLY MORE verbs to a sense
// 2.4 search PROPs based on verbs (both HAVING verb and NOT HAVING verb)
// 2.5 (one more thing: all searches can be multi-stage filtering search, so you can apply new criteria to remaining items)
// NOTE you might fail to do the entailment/mutex editing when the new changes are in contradiction with the existing PROPS.
// e.g. you have one short note assigned 2 verbs, v1 and v2, if you try to add mutex group including v1 and v2, it fails
// thus you need to provide information to user about the PROPs making the change failed


