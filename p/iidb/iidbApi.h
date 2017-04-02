
#include "iidbPhysical.h"


#include "iidbSpecialized.h"
#include "moduleuuid.h"
#include "portnumbers.h"

/**
 *  these 2 functions are used when you hope you can change the sense denoted by an ID.
 *  you pass all props related to the new sense.
 *  search iidb, if the old sense exists in props other than the passed ones, you cannot discard the old sense
 *  in this case a new id is necessary, whether to use the new id for old sense/new sense depends on which side is easier to modify
 *  chkNewIdForExistingPropsNecessaryOrCountLeftOut probably won't be used coz use old id for old sense seems more natural
 */
static int chkNewIdForExistingPropsNecessary(const sqlite3_int64 oldid, const size_t argAndParamCount, const sqlite3_int64 *argsAndParams, int *anyLeftOutProps) {
    //undone find all tables that OLDID can possibly appear in (this involves things like is-a props)
    //undone select from all those tables where col?=oldid and (col?,col?,col?) not in (?,?,?)
    //undone if there are any results, then:
    *anyLeftOutProps = 1;
    return RETVALSUCCESS;

    //undone if nothing found,then :
    *anyLeftOutProps = 0;
    return RETVALSUCCESS;
}

static int chkNewIdForExistingPropsNecessaryOrCountLeftOut(const sqlite3_int64 oldid, const size_t argAndParamCount, const sqlite3_int64 *argsAndParams, int *numOfLeftOutProps) {
    *numOfLeftOutProps = 0;
    //undone find all tables that OLDID can possibly appear in (this involves things like is-a props)
    //undone select from all those tables where col?=oldid and (col?,col?,col?) not in (?,?,?)
    //undone for each result:
    (*numOfLeftOutProps)++;
    return RETVALSUCCESS;
}

static int reassignBytesSenseToIdOrUseNewId(sqlite3_int64 sid, const unsigned char *newBytes, const size_t argAndParamCount, const sqlite3_int64 *argsAndParams) {
    //int anyLeftOutProps;
    //undone call chkNewIdForExistingPropsNecessary
    //if (anyLeftOutProps) {
        //undone update bytessensemap
    //} else {
    //    retAvailGlobalId(&sid);
        //undone insert into bytessensemap, bind newBytes
        //undone and then update all passed props, bind sid
    //}
    return RETVALSUCCESS;
}

static int addEntailment(sqlite3_int64 *premiseConjoinArg, sqlite3_int64 *consequConjoinArg){//undone more args about externalization
    //select max(c) from formulaorconj
    //select max(p) from formulaorform
    //new formula ref no
    //2 new prop ref numbers
    //foreach premiseConjoinArg int64, select from argvermapping,
    //insert into formulaorconj values()
    return RETVALFAILURE;
}

static void savunknowndat(void);

//note 126 is a reserved code for stopping server!!

#define URLBOOKMARK_VSEARCH 162
#define SELECTVERBSTHATDONOTENTAIL 163
#define CHKPROPSVALIDITY 164
#define DELETE_MUTEX 165
#define UPSERT_MUTEX 166
#define SELECT_MUTEXGROUP 167
#define SELECTMUTEXCONTAININGVERB 168
#define INSERT_ENTAILMENT 169
#define DELETE_ENTAILMENT 170
#define SELECTVERBSDIRECTLYENTAILEDBYTHIS 171
#define SELECTVERBSDIRECTLYENTAILINGTHIS 172
#define DELETE_SAVERB 173
#define SHORTNOTES_VSEARCH 174
#define UPSERT_URLBOOKMARK 175
#define SELECT_URLBOOKMARK 176
#define DELETE_URLBOOKMARK 177
#define SEARCH_URLBOOKMARK 178
#define UPSERTORDELETE_WORDSENSEGROUP 179
#define SELECT_ESSENTIALTEXT 180
#define SEARCH_WORDSENSEGROUP 181
#define SELECT_SAVERB 182
#define UPSERT_SAVERB 183
#define DELETE_ANIMATION 184
#define UPSERT_ANIMATION 185
#define SELECT_SAPROP 186
#define APPLICABLEGENERALVERBS 187
#define SAVESAPROPS 188
#define SEARCH_VERBDESC 189
#define DELETE_SHORTNOTE 190
#define SELECT_SHORTNOTE 191
#define SEARCH_SHORTNOTETEXT 192
#define UPSERT_SHORTNOTETEXTONLY 193
#define DELETE_FROMFILESTNDATASTINFO 194
#define INSERT_DATASTOREINFO 195
#define INSERT_FILEALLOCNAME 196
#define UPSERT_MALANIME 197
#define CHKANIMEREGISTRATION 198
#define SELECT_ANIME 199
#define SEARCH_ANIMEWITHFILE 200


static int analyzeRecvBuf(unsigned char uc) {
    int retintbuf;
    switch (uc) {
        case URLBOOKMARK_VSEARCH:
            retintbuf = tempSpUrlBookmarkVSearch();
            break;
        case SELECTVERBSTHATDONOTENTAIL:
            retintbuf = tempSpSelectVerbsThatDoNotEntail();
            break;
        case CHKPROPSVALIDITY:
            retintbuf = tempSpChkPropsValidity();
            break;
        case DELETE_MUTEX:
            retintbuf = tempSpDeleteMutex();
            break;
        case UPSERT_MUTEX:
            retintbuf = tempSpUpsertMutex();
            break;
        case SELECT_MUTEXGROUP:
            retintbuf = tempSpSelectMutexGroup();
            break;
        case SELECTMUTEXCONTAININGVERB:
            retintbuf = tempSpSelectMutexContainingVerb();
            break;
        case INSERT_ENTAILMENT:
            retintbuf = tempSpInsertEntailment();
            break;
        case DELETE_ENTAILMENT:
            retintbuf = tempSpDeleteEntailment();
            break;
        case SELECTVERBSDIRECTLYENTAILEDBYTHIS:
            retintbuf = tempSpSelectVerbsDirectlyEntailedByThis();
            break;
        case SELECTVERBSDIRECTLYENTAILINGTHIS:
            retintbuf = tempSpSelectVerbsDirectlyEntailingThis();
            break;
        case DELETE_SAVERB:
            retintbuf = tempSpDeleteSAVerb();
            break;
        case SHORTNOTES_VSEARCH:
            retintbuf = tempSpShortNotesVSearch();
            break;
        case UPSERT_URLBOOKMARK:
            retintbuf = tempSpUpsertUrlBookmark();
            break;
        case SELECT_URLBOOKMARK:
            retintbuf = tempSpSelectUrlBookmark();
            break;
        case DELETE_URLBOOKMARK:
            retintbuf = tempSpDeleteUrlBookmark();
            break;
        case SEARCH_URLBOOKMARK:
            retintbuf = tempSpSearchDescNUrlInUrlBookmark();
            break;
        case UPSERTORDELETE_WORDSENSEGROUP:
            retintbuf = tempSpUpsertOrDeleteWordSenseGroup();
            break;
        case SELECT_ESSENTIALTEXT:
            retintbuf = tempSpSelectEssentialText();
            break;
        case SEARCH_WORDSENSEGROUP:
            retintbuf = tempSpSearchWordSenseGroup();
            break;
        case SELECT_SAVERB:
            retintbuf = tempSpSelectSAVerb();
            break;
        case UPSERT_SAVERB:
            retintbuf = tempSpUpsertSAVerb();
            break;
        case DELETE_ANIMATION:
            retintbuf = tempSpDeleteAnimation();
            break;
        case UPSERT_ANIMATION:
            retintbuf = tempSpUpsertAnimation();
            break;
        case SELECT_SAPROP:
            retintbuf = tempSpSelectSAProp();
            break;
        case APPLICABLEGENERALVERBS:
            retintbuf = tempSpApplicableGeneralVerbs();
            break;
        case SAVESAPROPS:
            retintbuf = tempSpSaveSAProps();
            break;
        case SEARCH_VERBDESC:
            retintbuf = tempSpSearchVerbDesc();
            break;
        case DELETE_SHORTNOTE:
            retintbuf = tempSpDeleteShortNote();
            break;
        case SELECT_SHORTNOTE:
            retintbuf = tempSpSelectShortNote();
            break;
        case SEARCH_SHORTNOTETEXT:
            retintbuf = tempSpSearchShortNoteText();
            break;
        case UPSERT_SHORTNOTETEXTONLY:
            retintbuf = tempSpUpsertShortNoteTextOnly();
            break;
        case DELETE_FROMFILESTNDATASTINFO:
            retintbuf = tempSpDeleteFromFileStNDataStInfo();
            break;
        case INSERT_DATASTOREINFO:
            retintbuf = tempSpInsertDataStoreInfo();
            break;
        case INSERT_FILEALLOCNAME:
            retintbuf = tempSpInsertFileAllocName();
            break;
        case UPSERT_MALANIME:
            retintbuf = tempSpUpsertMalAnime();
            break;
        case CHKANIMEREGISTRATION:
            retintbuf = tempSpChkAnimeRegistration();
            break;
        case SELECT_ANIME:
            retintbuf = tempSpSelectAnime();
            break;
        case SEARCH_ANIMEWITHFILE:
            retintbuf = tempSpSearchAnimeWithFile();
            break;
        default:
            #define GETCHARTILLLFs(cbuf,ifchar,ifnotchar) cbuf=getchar();if(cbuf=='\n'){ifnotchar}else if(getchar()=='\n'){ifchar}else{while(getchar()!='\n'){}ifnotchar}
            LOGLITERALs("Unknown action received, the action code is:")
            //printf("%hhu\n",uc);
	    retintbuf = uc;
	    LOGERRs
	    StopIIMgmtSysServer();
	    puts("press any key to continue.");
            char cbuf;GETCHARTILLLFs(cbuf,,)

            savunknowndat();
            retintbuf = RETVALFAILURE;
            break;
    }

    //undone
    return retintbuf;
}
