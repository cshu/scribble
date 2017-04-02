
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "sqlite3.h"
#include "SNLTypes.h"
#include "SNL.h"

int(*sendUntilBytesPtr)(const unsigned char *, size_t);
int(*recvUntilBytesPtr)(unsigned char *, size_t);

static int GetPredTabNameFromVerbSegId(sqlite3_int64, const unsigned char **, size_t *);
static int GetPredTabNameFromNounSegId(sqlite3_int64, const unsigned char **, size_t *);
static int IterateAllNounEndsUnderVerbSeg(sqlite3_int64, int(*)(sqlite3_int64));
static int IterateAllNounEndsUnderNounSeg(sqlite3_int64, int(*)(sqlite3_int64));

//static sqlite3_int64 lastInsertNewInt64;
static unsigned char *recvbuf;

//todo: load the paths from config file.
static sqlite3 *databaseA = NULL;
//static sqlite3_stmt *selectFromDetermadj;
static sqlite3_stmt *selectFromInvertiblev;
static sqlite3_stmt *selectFromNoninvertib;
static sqlite3_stmt *selectFromBytnoun;
static sqlite3_stmt *selectFromDignoun;
static sqlite3_stmt *selectFromConroot;
static sqlite3_stmt *selectFromAbsroot;
static sqlite3_stmt *selectFromMutexcl2;
static sqlite3_stmt *selectFromMutexcl3;
static sqlite3_stmt *selectFromSubcate1;
static sqlite3_stmt *selectFromSubcate2;
static sqlite3_stmt *selectFromInter2;
static sqlite3_stmt *selectFromUnion2;
static sqlite3_stmt *selectFromInter3;
static sqlite3_stmt *selectFromUnion3;
static sqlite3_stmt *selectFromRelacomp2;
static sqlite3_stmt *selectFromRelacomp3;
static sqlite3_stmt *selectFromSynonym;
static sqlite3_stmt *selectFromInverseverb;

static sqlite3_stmt *selectGreatestTabNameFromIndepcl;
static sqlite3_stmt *selectGreatestNounSegID;
static sqlite3_stmt *selectGreatestVerbSegID;
static sqlite3_stmt *selectGreatestInsID;
static sqlite3_stmt *selectSegIDsWhereSenseID;
static sqlite3_stmt *selectTabNameWhereNounSegID;
static sqlite3_stmt *selectParentSegIdFromNounasWhereNounSegId;
static sqlite3_stmt *selectParentSegIdFromGerundWhereNounSegId;
static sqlite3_stmt *selectTabNameWhereVerbSegID;
static sqlite3_stmt *selectParentSegIdFromNounasWhereVerbSegId;
static sqlite3_stmt *selectParentSegIdFromGerundWhereVerbSegId;
static sqlite3_stmt *selectFromRelativeclaWhereSegId;
static sqlite3_stmt *selectChildrenSegFromNounClauWhereParent;
static sqlite3_stmt *segExistInVar;
static sqlite3_stmt *selectFromAdverbialclWhereSegId;
static sqlite3_stmt *selectChildrenSegFromVerbClauWhereParent;
static sqlite3_stmt *selectFromIndepclWhereTabname;
static sqlite3_stmt *segExistInVarSelectSense;
static sqlite3_stmt *selectGreatestSNLSenseId;

static sqlite3_stmt *selectFromIndepcl;
static sqlite3_stmt *selectFromConnounvar;
static sqlite3_stmt *selectFromConnounins;
static sqlite3_stmt *selectFromBytnounvar;
static sqlite3_stmt *selectFromBytnounins;
static sqlite3_stmt *selectFromDignounvar;
static sqlite3_stmt *selectFromDignounins;
static sqlite3_stmt *selectFromRawbytevar;
static sqlite3_stmt *selectFromRawbyteins;
static sqlite3_stmt *selectFromRawdigivar;
static sqlite3_stmt *selectFromRawdigiins;
static sqlite3_stmt *selectFromNormalverb;
static sqlite3_stmt *selectFromNormalnoun;
static sqlite3_stmt *selectFromAdverbialcl;
static sqlite3_stmt *selectFromRelativecla;
static sqlite3_stmt *selectFromGerundasobj;
static sqlite3_stmt *selectFromGerundassub;
static sqlite3_stmt *selectFromNounasobjec;
static sqlite3_stmt *selectFromNounassubje;

static sqlite3_stmt *insertIntoInvertiblev;
static sqlite3_stmt *insertIntoNoninvertib;
static sqlite3_stmt *insertIntoBytnoun;
static sqlite3_stmt *insertIntoDignoun;
static sqlite3_stmt *insertIntoConroot;
static sqlite3_stmt *insertIntoAbsroot;
static sqlite3_stmt *insertIntoMutexcl2;
static sqlite3_stmt *insertIntoMutexcl3;
static sqlite3_stmt *insertIntoSubcate1;
static sqlite3_stmt *insertIntoSubcate2;
static sqlite3_stmt *insertIntoInter2;
static sqlite3_stmt *insertIntoUnion2;
static sqlite3_stmt *insertIntoInter3;
static sqlite3_stmt *insertIntoUnion3;
static sqlite3_stmt *insertIntoRelacomp2;
static sqlite3_stmt *insertIntoRelacomp3;
static sqlite3_stmt *insertIntoSynonym;
static sqlite3_stmt *insertIntoInverseverb;

static sqlite3_stmt *insertIntoIndepcl;
static sqlite3_stmt *insertIntoConnounvar;
static sqlite3_stmt *insertIntoConnounins;
static sqlite3_stmt *insertIntoBytnounvar;
static sqlite3_stmt *insertIntoBytnounins;
static sqlite3_stmt *insertIntoDignounvar;
static sqlite3_stmt *insertIntoDignounins;
static sqlite3_stmt *insertIntoRawbytevar;
static sqlite3_stmt *insertIntoRawbyteins;
static sqlite3_stmt *insertIntoRawdigivar;
static sqlite3_stmt *insertIntoRawdigiins;
static sqlite3_stmt *insertIntoNormalverb;
static sqlite3_stmt *insertIntoNormalnoun;
static sqlite3_stmt *insertIntoAdverbialcl;
static sqlite3_stmt *insertIntoRelativecla;
static sqlite3_stmt *insertIntoGerundasobj;
static sqlite3_stmt *insertIntoGerundassub;
static sqlite3_stmt *insertIntoNounasobjec;
static sqlite3_stmt *insertIntoNounassubje;

struct SiDarr {
    sqlite3_int64 *si;
    size_t length;
    size_t capacity;
};

static struct SiDarr out_arr_GetSenseIdFromPredVarIndex;
static size_t out_iOfGreatest_GetSenseIdFromPredVarIndex;
static size_t in_i_GetSenseIdFromPredVarIndex;
static struct SiDarr out_arr_GetNumOfVarLessThanSegUnderPred;
static sqlite3_int64 in_seg_GetNumOfVarLessThanSegUnderPred;

void SiDarrAddMem(sqlite3_int64 mem, struct SiDarr *sd) {
    if (sd->length == sd->capacity) {
        sd->capacity *= 2;
        sqlite3_int64 *ptr = realloc(sd->si, sd->capacity * (sizeof *sd->si));
        //todo: check ptr == NULL
        sd->si = ptr;
    }
    sd->si[sd->length] = mem;
    sd->length++;
}

size_t SiDarrIndexOf(sqlite3_int64 mem, struct SiDarr *sd) {
    for (size_t i = 0; i < sd->length; i++) {
        if (sd->si[i] == mem) {
            return i;
        }
    }
    return sd->length;
}

sqlite3_int64 derefUcGetSi(const unsigned char *uc){
    sqlite3_int64 si;
    memcpy(&si, uc, sizeof si);
    return si;
}

void errorLogCallback(void *pArg, int iErrCode, const char *zMsg){
    printf("(%d) %s\n", iErrCode, zMsg);
}

void IIDAL2_free(void *ptr) {
    free(ptr);
}

int GetAllPredicatesData() {
    sqlite3_int64 int64buffer;
    int retval;
    int intBuf;
    size_t tabNameLen;
    const unsigned char *tabName;
#define SELECTFROM_I(itab,typenum)\
        retval = sqlite3_step(selectFrom##itab);\
        /*todo: check return value*/\
        if (retval != SQLITE_ROW){\
            if(retval != SQLITE_DONE){\
                printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
                if(retval == SQLITE_ERROR)\
                    printf("%s\n",sqlite3_errmsg);\
            }\
            break;\
        }\
        sendUntilBytesPtr((unsigned char *)typenum, 1);\
        int64buffer = sqlite3_column_int64(selectFrom##itab, 0);\
        sendUntilBytesPtr((unsigned char *) &int64buffer, 8)
#define SELECTFROM_II(itab,typenum)\
        SELECTFROM_I(itab,typenum);\
        int64buffer = sqlite3_column_int64(selectFrom##itab, 1);\
        sendUntilBytesPtr((unsigned char *) &int64buffer, 8)
#define SELECTFROM_III(itab,typenum)\
        SELECTFROM_II(itab,typenum);\
        int64buffer = sqlite3_column_int64(selectFrom##itab, 2);\
        sendUntilBytesPtr((unsigned char *) &int64buffer, 8)
#define SELECTFROM_IB(itab,typenum)\
        SELECTFROM_I(itab,typenum);\
        intBuf = sqlite3_column_bytes(selectFrom##itab, 1);\
        sendUntilBytesPtr((unsigned char *) &intBuf, 2);\
        sendUntilBytesPtr(sqlite3_column_blob(selectFrom##itab, 1), intBuf)
#define SELECTFROM_IIB(itab,typenum)\
        SELECTFROM_II(itab,typenum);\
        intBuf = sqlite3_column_bytes(selectFrom##itab, 2);\
        sendUntilBytesPtr((unsigned char *) &intBuf, 2);\
        sendUntilBytesPtr(sqlite3_column_blob(selectFrom##itab, 2), intBuf)
#define SELECTFROM_IIIB(itab,typenum)\
        SELECTFROM_III(itab,typenum);\
        intBuf = sqlite3_column_bytes(selectFrom##itab, 3);\
        sendUntilBytesPtr((unsigned char *) &intBuf, 2);\
        sendUntilBytesPtr(sqlite3_column_blob(selectFrom##itab, 3), intBuf)
    sqlite3_reset(selectFromIndepcl);
    for (;;) {
        SELECTFROM_IIIB(Indepcl, "\x02");
    }
    sqlite3_reset(selectFromConnounvar);
    for (;;) {
        SELECTFROM_II(Connounvar, "\x03");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromConnounvar, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromConnounins);
    for (;;) {
        SELECTFROM_III(Connounins, "\x04");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromConnounins, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromBytnounvar);
    for (;;) {
        SELECTFROM_II(Bytnounvar, "\x05");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromBytnounvar, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromBytnounins);
    for (;;) {
        SELECTFROM_IIB(Bytnounins, "\x06");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromBytnounins, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromDignounvar);
    for (;;) {
        SELECTFROM_II(Dignounvar, "\x07");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromDignounvar, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromDignounins);
    for (;;) {
        SELECTFROM_IIB(Dignounins, "\x08");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromDignounins, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromRawbytevar);
    for (;;) {
        SELECTFROM_I(Rawbytevar, "\x09");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromRawbytevar, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromRawbyteins);
    for (;;) {
        SELECTFROM_IB(Rawbyteins, "\x0A");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromRawbyteins, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromRawdigivar);
    for (;;) {
        SELECTFROM_I(Rawdigivar, "\x0B");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromRawdigivar, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromRawdigiins);
    for (;;) {
        SELECTFROM_IB(Rawdigiins, "\x0C");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromRawdigiins, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromNormalverb);
    for (;;) {
        SELECTFROM_II(Normalverb, "\x0D");
        GetPredTabNameFromVerbSegId(sqlite3_column_int64(selectFromNormalverb, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromNormalnoun);
    for (;;) {
        SELECTFROM_II(Normalnoun, "\x0E");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromNormalnoun, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromAdverbialcl);
    for (;;) {
        SELECTFROM_II(Adverbialcl, "\x0F");
        GetPredTabNameFromVerbSegId(sqlite3_column_int64(selectFromAdverbialcl, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromRelativecla);
    for (;;) {
        SELECTFROM_II(Relativecla, "\x10");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromRelativecla, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromGerundasobj);
    for (;;) {
        SELECTFROM_III(Gerundasobj, "\x11");
        GetPredTabNameFromVerbSegId(sqlite3_column_int64(selectFromGerundasobj, 2), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromGerundassub);
    for (;;) {
        SELECTFROM_III(Gerundassub, "\x12");
        GetPredTabNameFromVerbSegId(sqlite3_column_int64(selectFromGerundassub, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromNounasobjec);
    for (;;) {
        SELECTFROM_III(Nounasobjec, "\x13");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromNounasobjec, 2), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
    sqlite3_reset(selectFromNounassubje);
    for (;;) {
        SELECTFROM_III(Nounassubje, "\x14");
        GetPredTabNameFromNounSegId(sqlite3_column_int64(selectFromNounassubje, 0), &tabName, &tabNameLen);
        sendUntilBytesPtr((unsigned char *) &tabNameLen, 2);
        sendUntilBytesPtr(tabName, tabNameLen);
    }
#undef SELECTFROM_I
#undef SELECTFROM_II
#undef SELECTFROM_III
#undef SELECTFROM_IB
#undef SELECTFROM_IIB
#undef SELECTFROM_IIIB
    sendUntilBytesPtr((unsigned char *)"\xFF", 1);
    return 0;
}

int GetAllISNLSenseTableData() {
    sqlite3_int64 int64buffer;
    int retval;
    int intBuf;
#define SELECTFROM_IB(itab)\
sqlite3_reset(selectFrom##itab);\
for (;;) {\
    retval = sqlite3_step(selectFrom##itab);\
    if (retval != SQLITE_ROW){\
        if(retval != SQLITE_DONE){\
            printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
            if(retval == SQLITE_ERROR)\
                printf("%s\n",sqlite3_errmsg);\
        }\
        break;\
    }\
    sendUntilBytesPtr((unsigned char *) "\x00", 1);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 0);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
    intBuf = sqlite3_column_bytes(selectFrom##itab, 1);\
    sendUntilBytesPtr((unsigned char *) &intBuf, 2);\
    sendUntilBytesPtr((unsigned char *) sqlite3_column_blob(selectFrom##itab, 1), intBuf);\
}\
sendUntilBytesPtr((unsigned char *) "\x01", 1)
#define SELECTFROM_II(itab)\
sqlite3_reset(selectFrom##itab);\
for (;;) {\
    retval = sqlite3_step(selectFrom##itab);\
    if (retval != SQLITE_ROW)\
        break;\
    sendUntilBytesPtr((unsigned char *) "\x00", 1);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 0);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 1);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
}\
sendUntilBytesPtr((unsigned char *) "\x01", 1)
#define SELECTFROM_III(itab)\
sqlite3_reset(selectFrom##itab);\
for (;;) {\
    retval = sqlite3_step(selectFrom##itab);\
    if (retval != SQLITE_ROW)\
        break;\
    sendUntilBytesPtr((unsigned char *) "\x00", 1);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 0);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 1);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 2);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
}\
sendUntilBytesPtr((unsigned char *) "\x01", 1)
#define SELECTFROM_IIII(itab)\
sqlite3_reset(selectFrom##itab);\
for (;;) {\
    retval = sqlite3_step(selectFrom##itab);\
    if (retval != SQLITE_ROW)\
        break;\
    sendUntilBytesPtr((unsigned char *) "\x00", 1);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 0);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 1);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 2);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 3);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
}\
sendUntilBytesPtr((unsigned char *) "\x01", 1)
#define SELECTFROM_BI(itab)\
sqlite3_reset(selectFrom##itab);\
for (;;) {\
    retval = sqlite3_step(selectFrom##itab);\
    if (retval != SQLITE_ROW)\
        break;\
    sendUntilBytesPtr((unsigned char *) "\x00", 1);\
    intBuf = sqlite3_column_bytes(selectFrom##itab, 0);\
    sendUntilBytesPtr((unsigned char *) &intBuf, 2);\
    sendUntilBytesPtr(sqlite3_column_blob(selectFrom##itab, 0), intBuf);\
    int64buffer = sqlite3_column_int64(selectFrom##itab, 1);\
    sendUntilBytesPtr((unsigned char *) &int64buffer, 8);\
}\
sendUntilBytesPtr((unsigned char *) "\x01", 1)
    SELECTFROM_IB(Invertiblev);
    SELECTFROM_IB(Noninvertib);
    SELECTFROM_IB(Bytnoun);
    SELECTFROM_IB(Dignoun);
    SELECTFROM_IB(Conroot);
    SELECTFROM_IB(Absroot);
    SELECTFROM_II(Mutexcl2);
    SELECTFROM_III(Mutexcl3);
    SELECTFROM_II(Subcate1);
    SELECTFROM_III(Subcate2);
    SELECTFROM_III(Inter2);
    SELECTFROM_III(Union2);
    SELECTFROM_IIII(Inter3);
    SELECTFROM_IIII(Union3);
    SELECTFROM_III(Relacomp2);
    SELECTFROM_IIII(Relacomp3);
    SELECTFROM_BI(Synonym);
    SELECTFROM_BI(Inverseverb);
#undef SELECTFROM_IB
#undef SELECTFROM_II
#undef SELECTFROM_III
#undef SELECTFROM_IIII
#undef SELECTFROM_BI
}

void FinalizeSqlite3Stmts(){
    
    //sqlite3_finalize(selectFromDetermadj);
    sqlite3_finalize(selectFromInvertiblev);
    sqlite3_finalize(selectFromNoninvertib);
    sqlite3_finalize(selectFromBytnoun);
    sqlite3_finalize(selectFromDignoun);
    sqlite3_finalize(selectFromConroot);
    sqlite3_finalize(selectFromAbsroot);
    sqlite3_finalize(selectFromMutexcl2);
    sqlite3_finalize(selectFromMutexcl3);
    sqlite3_finalize(selectFromSubcate1);
    sqlite3_finalize(selectFromSubcate2);
    sqlite3_finalize(selectFromInter2);
    sqlite3_finalize(selectFromUnion2);
    sqlite3_finalize(selectFromInter3);
    sqlite3_finalize(selectFromUnion3);
    sqlite3_finalize(selectFromRelacomp2);
    sqlite3_finalize(selectFromRelacomp3);
    sqlite3_finalize(selectFromSynonym);
    sqlite3_finalize(selectFromInverseverb);


    sqlite3_finalize(selectGreatestTabNameFromIndepcl);
    sqlite3_finalize(selectGreatestNounSegID);
    sqlite3_finalize(selectGreatestVerbSegID);
    sqlite3_finalize(selectGreatestInsID);
    sqlite3_finalize(selectSegIDsWhereSenseID);
    sqlite3_finalize(selectTabNameWhereNounSegID);
    sqlite3_finalize(selectParentSegIdFromNounasWhereNounSegId);
    sqlite3_finalize(selectParentSegIdFromGerundWhereNounSegId);
    sqlite3_finalize(selectTabNameWhereVerbSegID);
    sqlite3_finalize(selectParentSegIdFromNounasWhereVerbSegId);
    sqlite3_finalize(selectParentSegIdFromGerundWhereVerbSegId);
    sqlite3_finalize(selectFromRelativeclaWhereSegId);
    sqlite3_finalize(selectChildrenSegFromNounClauWhereParent);
    sqlite3_finalize(segExistInVar);
    sqlite3_finalize(selectFromAdverbialclWhereSegId);
    sqlite3_finalize(selectChildrenSegFromVerbClauWhereParent);
    sqlite3_finalize(selectFromIndepclWhereTabname);
    sqlite3_finalize(segExistInVarSelectSense);
    sqlite3_finalize(selectGreatestSNLSenseId);

    sqlite3_finalize(selectFromIndepcl);
    sqlite3_finalize(selectFromConnounvar);
    sqlite3_finalize(selectFromConnounins);
    sqlite3_finalize(selectFromBytnounvar);
    sqlite3_finalize(selectFromBytnounins);
    sqlite3_finalize(selectFromDignounvar);
    sqlite3_finalize(selectFromDignounins);
    sqlite3_finalize(selectFromRawbytevar);
    sqlite3_finalize(selectFromRawbyteins);
    sqlite3_finalize(selectFromRawdigivar);
    sqlite3_finalize(selectFromRawdigiins);
    sqlite3_finalize(selectFromNormalverb);
    sqlite3_finalize(selectFromNormalnoun);
    sqlite3_finalize(selectFromAdverbialcl);
    sqlite3_finalize(selectFromRelativecla);
    sqlite3_finalize(selectFromGerundasobj);
    sqlite3_finalize(selectFromGerundassub);
    sqlite3_finalize(selectFromNounasobjec);
    sqlite3_finalize(selectFromNounassubje);

    sqlite3_finalize(insertIntoInvertiblev);
    sqlite3_finalize(insertIntoNoninvertib);
    sqlite3_finalize(insertIntoBytnoun);
    sqlite3_finalize(insertIntoDignoun);
    sqlite3_finalize(insertIntoConroot);
    sqlite3_finalize(insertIntoAbsroot);
    sqlite3_finalize(insertIntoMutexcl2);
    sqlite3_finalize(insertIntoMutexcl3);
    sqlite3_finalize(insertIntoSubcate1);
    sqlite3_finalize(insertIntoSubcate2);
    sqlite3_finalize(insertIntoInter2);
    sqlite3_finalize(insertIntoUnion2);
    sqlite3_finalize(insertIntoInter3);
    sqlite3_finalize(insertIntoUnion3);
    sqlite3_finalize(insertIntoRelacomp2);
    sqlite3_finalize(insertIntoRelacomp3);
    sqlite3_finalize(insertIntoSynonym);
    sqlite3_finalize(insertIntoInverseverb);

    sqlite3_finalize(insertIntoIndepcl);
    sqlite3_finalize(insertIntoConnounvar);
    sqlite3_finalize(insertIntoConnounins);
    sqlite3_finalize(insertIntoBytnounvar);
    sqlite3_finalize(insertIntoBytnounins);
    sqlite3_finalize(insertIntoDignounvar);
    sqlite3_finalize(insertIntoDignounins);
    sqlite3_finalize(insertIntoRawbytevar);
    sqlite3_finalize(insertIntoRawbyteins);
    sqlite3_finalize(insertIntoRawdigivar);
    sqlite3_finalize(insertIntoRawdigiins);
    sqlite3_finalize(insertIntoNormalverb);
    sqlite3_finalize(insertIntoNormalnoun);
    sqlite3_finalize(insertIntoAdverbialcl);
    sqlite3_finalize(insertIntoRelativecla);
    sqlite3_finalize(insertIntoGerundasobj);
    sqlite3_finalize(insertIntoGerundassub);
    sqlite3_finalize(insertIntoNounasobjec);
    sqlite3_finalize(insertIntoNounassubje);
}
void PrepareSqlite3Stmts(){
    
    int retval;
    //retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM determadj", -1, &selectFromDetermadj, NULL);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM invertiblev", -1, &selectFromInvertiblev, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM noninvertib", -1, &selectFromNoninvertib, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM bytnoun", -1, &selectFromBytnoun, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM dignoun", -1, &selectFromDignoun, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM conroot", -1, &selectFromConroot, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM absroot", -1, &selectFromAbsroot, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM mutexcl2", -1, &selectFromMutexcl2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM mutexcl3", -1, &selectFromMutexcl3, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM subcate1", -1, &selectFromSubcate1, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM subcate2", -1, &selectFromSubcate2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM inter2", -1, &selectFromInter2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM union2", -1, &selectFromUnion2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM inter3", -1, &selectFromInter3, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM union3", -1, &selectFromUnion3, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM relacomp2", -1, &selectFromRelacomp2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM relacomp3", -1, &selectFromRelacomp3, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM synonym", -1, &selectFromSynonym, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM inverseverb", -1, &selectFromInverseverb, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);

    retval = sqlite3_prepare_v2(databaseA, "SELECT d FROM indepcl ORDER BY LENGTH(d) DESC, d DESC LIMIT 1", -1, &selectGreatestTabNameFromIndepcl, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT a FROM indeppr UNION ALL SELECT c FROM indeppr UNION ALL SELECT a FROM indepcl UNION ALL SELECT c FROM indepcl UNION ALL SELECT a FROM gerundasobj UNION ALL SELECT c FROM gerundassub UNION ALL SELECT a FROM nounasobjec UNION ALL SELECT c FROM nounassubje ORDER BY 1 DESC LIMIT 1", -1, &selectGreatestNounSegID, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT b FROM indeppr UNION ALL SELECT b FROM indepcl UNION ALL SELECT b FROM gerundasobj UNION ALL SELECT b FROM gerundassub UNION ALL SELECT b FROM nounasobjec UNION ALL SELECT b FROM nounassubje ORDER BY 1 DESC LIMIT 1", -1, &selectGreatestVerbSegID, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT c FROM connounins WHERE b=? ORDER BY c DESC LIMIT 1", -1, &selectGreatestInsID, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT a FROM connounvar WHERE b=?", -1, &selectSegIDsWhereSenseID, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT d FROM indepcl WHERE a=? OR c=? LIMIT 1", -1, &selectTabNameWhereNounSegID, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT c FROM (SELECT c,a FROM nounasobjec UNION ALL SELECT a,c FROM nounassubje) WHERE a=? LIMIT 1", -1, &selectParentSegIdFromNounasWhereNounSegId, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT c FROM (SELECT c,a FROM gerundasobj UNION ALL SELECT a,c FROM gerundassub) WHERE a=? LIMIT 1", -1, &selectParentSegIdFromGerundWhereNounSegId, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT d FROM indepcl WHERE b=? LIMIT 1", -1, &selectTabNameWhereVerbSegID, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT c FROM (SELECT c,b FROM nounasobjec UNION ALL SELECT a,b FROM nounassubje) WHERE b=? LIMIT 1", -1, &selectParentSegIdFromNounasWhereVerbSegId, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT c FROM (SELECT c,b FROM gerundasobj UNION ALL SELECT a,b FROM gerundassub) WHERE b=? LIMIT 1", -1, &selectParentSegIdFromGerundWhereVerbSegId, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM relativecla WHERE a=?", -1, &selectFromRelativeclaWhereSegId, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT a,b FROM (SELECT a,b,c FROM nounasobjec UNION ALL SELECT c,b,a FROM nounassubje) WHERE c=?", -1, &selectChildrenSegFromNounClauWhereParent, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT a FROM (SELECT a FROM connounvar UNION ALL SELECT a FROM bytnounvar UNION ALL SELECT a FROM dignounvar UNION ALL SELECT a FROM rawbytevar UNION ALL SELECT a FROM rawdigivar) WHERE a=? LIMIT 1", -1, &segExistInVar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM adverbialcl WHERE a=?", -1, &selectFromAdverbialclWhereSegId, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT a,b FROM (SELECT a,b,c FROM gerundasobj UNION ALL SELECT c,b,a FROM gerundassub) WHERE c=?", -1, &selectChildrenSegFromVerbClauWhereParent, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT a,b,c FROM indepcl WHERE d=?", -1, &selectFromIndepclWhereTabname, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT b FROM (SELECT b,a FROM connounvar UNION ALL SELECT 0,a FROM bytnounvar UNION ALL SELECT 0,a FROM dignounvar UNION ALL SELECT 0,a FROM rawbytevar UNION ALL SELECT 0,a FROM rawdigivar) WHERE a=? LIMIT 1", -1, &segExistInVarSelectSense, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT b FROM synonym UNION ALL SELECT b FROM inverseverb ORDER BY 1 DESC LIMIT 1", -1, &selectGreatestSNLSenseId, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);

    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM indepcl", -1, &selectFromIndepcl, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM connounvar", -1, &selectFromConnounvar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM connounins", -1, &selectFromConnounins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM bytnounvar", -1, &selectFromBytnounvar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM bytnounins", -1, &selectFromBytnounins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM dignounvar", -1, &selectFromDignounvar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM dignounins", -1, &selectFromDignounins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM rawbytevar", -1, &selectFromRawbytevar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM rawbyteins", -1, &selectFromRawbyteins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM rawdigivar", -1, &selectFromRawdigivar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM rawdigiins", -1, &selectFromRawdigiins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM normalverb", -1, &selectFromNormalverb, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM normalnoun", -1, &selectFromNormalnoun, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM adverbialcl", -1, &selectFromAdverbialcl, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM relativecla", -1, &selectFromRelativecla, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM gerundasobj", -1, &selectFromGerundasobj, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM gerundassub", -1, &selectFromGerundassub, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM nounasobjec", -1, &selectFromNounasobjec, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "SELECT * FROM nounassubje", -1, &selectFromNounassubje, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);

    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO invertiblev VALUES(?,?)", -1, &insertIntoInvertiblev, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO noninvertib VALUES(?,?)", -1, &insertIntoNoninvertib, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO bytnoun VALUES(?,?)", -1, &insertIntoBytnoun, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO dignoun VALUES(?,?)", -1, &insertIntoDignoun, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO conroot VALUES(?,?)", -1, &insertIntoConroot, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO absroot VALUES(?,?)", -1, &insertIntoAbsroot, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO mutexcl2 VALUES(?,?)", -1, &insertIntoMutexcl2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO mutexcl3 VALUES(?,?,?)", -1, &insertIntoMutexcl3, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO subcate1 VALUES(?,?)", -1, &insertIntoSubcate1, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO subcate2 VALUES(?,?,?)", -1, &insertIntoSubcate2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO inter2 VALUES(?,?,?)", -1, &insertIntoInter2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO union2 VALUES(?,?,?)", -1, &insertIntoUnion2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO inter3 VALUES(?,?,?,?)", -1, &insertIntoInter3, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO union3 VALUES(?,?,?,?)", -1, &insertIntoUnion3, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO relacomp2 VALUES(?,?,?)", -1, &insertIntoRelacomp2, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO relacomp3 VALUES(?,?,?,?)", -1, &insertIntoRelacomp3, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO synonym VALUES(?,?)", -1, &insertIntoSynonym, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO inverseverb VALUES(?,?)", -1, &insertIntoInverseverb, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);

    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO indepcl VALUES(?,?,?,?)", -1, &insertIntoIndepcl, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO connounvar VALUES(?,?)", -1, &insertIntoConnounvar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO connounins VALUES(?,?,?)", -1, &insertIntoConnounins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO bytnounvar VALUES(?,?)", -1, &insertIntoBytnounvar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO bytnounins VALUES(?,?,?)", -1, &insertIntoBytnounins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO dignounvar VALUES(?,?)", -1, &insertIntoDignounvar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO dignounins VALUES(?,?,?)", -1, &insertIntoDignounins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO rawbytevar VALUES(?)", -1, &insertIntoRawbytevar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO rawbyteins VALUES(?,?)", -1, &insertIntoRawbyteins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO rawdigivar VALUES(?)", -1, &insertIntoRawdigivar, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO rawdigiins VALUES(?,?)", -1, &insertIntoRawdigiins, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO normalverb VALUES(?,?)", -1, &insertIntoNormalverb, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO normalnoun VALUES(?,?)", -1, &insertIntoNormalnoun, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO adverbialcl VALUES(?,?)", -1, &insertIntoAdverbialcl, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO relativecla VALUES(?,?)", -1, &insertIntoRelativecla, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO gerundasobj VALUES(?,?,?)", -1, &insertIntoGerundasobj, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO gerundassub VALUES(?,?,?)", -1, &insertIntoGerundassub, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO nounasobjec VALUES(?,?,?)", -1, &insertIntoNounasobjec, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO nounassubje VALUES(?,?,?)", -1, &insertIntoNounassubje, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);

}

int SQLiteConfigOpenPragmaBeginTran() {
    recvbuf = malloc(50000);
    char *zErrMsg = NULL;
    
    sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, NULL);
    sqlite3_open(DATABASEA_FULLFILENAME, &databaseA);

    sqlite3_exec(databaseA, "PRAGMA locking_mode = EXCLUSIVE", NULL, NULL, &zErrMsg);
    sqlite3_exec(databaseA, "PRAGMA synchronous = OFF", NULL, NULL, &zErrMsg);
    sqlite3_exec(databaseA, "PRAGMA journal_mode = OFF", NULL, NULL, &zErrMsg);
    sqlite3_exec(databaseA, "PRAGMA temp_store = MEMORY", NULL, NULL, &zErrMsg);
    if (sqlite3_exec(databaseA, "BEGIN EXCLUSIVE", NULL, NULL, &zErrMsg))
        printf("databaseA %s\n", zErrMsg);

    if (zErrMsg != NULL) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    PrepareSqlite3Stmts();
    return 0;
}

int SQLiteCommitTranClose() {
    free(recvbuf);
    FinalizeSqlite3Stmts();

    char *zErrMsg = NULL;

    if (sqlite3_exec(databaseA, "COMMIT", NULL, NULL, &zErrMsg))
        printf("databaseA %s\n", zErrMsg);
    //todo: do something if error occurred
    if (zErrMsg != NULL) {
        free(zErrMsg);
    }

    if (sqlite3_close(databaseA) == SQLITE_OK) {
        databaseA = NULL;
        return 0;
    } else {
        return 1;
    }
}

int SQLiteCommitTranBeinTran() {
    //todo: commit changes
    return EXIT_SUCCESS;
}

int ExecDefragmentationForII() {
    //todo:1.remove unused records, 2.remove unused bytes from file, rewrite
    return EXIT_SUCCESS;
}

static int IterateAllNounEndsUnderNounSeg(sqlite3_int64 nounSeg, int(*Callback)(sqlite3_int64)) {
    int retval;
    sqlite3_clear_bindings(selectFromRelativeclaWhereSegId);
    sqlite3_reset(selectFromRelativeclaWhereSegId);
    retval = sqlite3_bind_int64(selectFromRelativeclaWhereSegId, 1, nounSeg);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectFromRelativeclaWhereSegId);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        sqlite3_clear_bindings(selectChildrenSegFromNounClauWhereParent);
        sqlite3_reset(selectChildrenSegFromNounClauWhereParent);
        retval = sqlite3_bind_int64(selectChildrenSegFromNounClauWhereParent, 1, nounSeg);
        if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
        for (;;) {
            retval = sqlite3_step(selectChildrenSegFromNounClauWhereParent);
            //todo: check return value
            if (retval != SQLITE_ROW) {
                break;
            }
            sqlite3_int64 recvNounSegId = sqlite3_column_int64(selectChildrenSegFromNounClauWhereParent, 0);
            sqlite3_int64 recvVerbSegId = sqlite3_column_int64(selectChildrenSegFromNounClauWhereParent, 1);
            IterateAllNounEndsUnderNounSeg(recvNounSegId, Callback);
            IterateAllNounEndsUnderVerbSeg(recvVerbSegId, Callback);
        }
    } else {
        Callback(nounSeg);
    }
    return 0;
}

static int IterateAllNounEndsUnderVerbSeg(sqlite3_int64 verbSeg, int(*Callback)(sqlite3_int64)) {
    int retval;
    sqlite3_clear_bindings(selectFromAdverbialclWhereSegId);
    sqlite3_reset(selectFromAdverbialclWhereSegId);
    retval = sqlite3_bind_int64(selectFromAdverbialclWhereSegId, 1, verbSeg);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectFromAdverbialclWhereSegId);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        sqlite3_clear_bindings(selectChildrenSegFromVerbClauWhereParent);
        sqlite3_reset(selectChildrenSegFromVerbClauWhereParent);
        retval = sqlite3_bind_int64(selectChildrenSegFromVerbClauWhereParent, 1, verbSeg);
        if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
        for (;;) {
            retval = sqlite3_step(selectChildrenSegFromVerbClauWhereParent);
            //todo: check return value
            if (retval != SQLITE_ROW) {
                break;
            }
            sqlite3_int64 recvNounSegId = sqlite3_column_int64(selectChildrenSegFromVerbClauWhereParent, 0);
            sqlite3_int64 recvVerbSegId = sqlite3_column_int64(selectChildrenSegFromVerbClauWhereParent, 1);
            IterateAllNounEndsUnderNounSeg(recvNounSegId, Callback);
            IterateAllNounEndsUnderVerbSeg(recvVerbSegId, Callback);
        }
    }
    return 0;
}

static int IterateAllNounEndsUnderPred(const unsigned char *tabName, size_t tabNameLen, int(*Callback)(sqlite3_int64)) {
    int retval;
    sqlite3_clear_bindings(selectFromIndepclWhereTabname);
    sqlite3_reset(selectFromIndepclWhereTabname);
    retval = sqlite3_bind_blob(selectFromIndepclWhereTabname, 1, tabName, tabNameLen, SQLITE_TRANSIENT);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    for (;;) {
        retval = sqlite3_step(selectFromIndepclWhereTabname);
        //todo: check return value
        if (retval != SQLITE_ROW) {
            return 0;
        }
        IterateAllNounEndsUnderNounSeg(sqlite3_column_int64(selectFromIndepclWhereTabname, 0), Callback);
        IterateAllNounEndsUnderVerbSeg(sqlite3_column_int64(selectFromIndepclWhereTabname, 1), Callback);
        IterateAllNounEndsUnderNounSeg(sqlite3_column_int64(selectFromIndepclWhereTabname, 2), Callback);
    }
}

static int Callback_GetSenseIdFromPredVarIndex(sqlite3_int64 nounEnd) {
    for (size_t i = 0; i < out_arr_GetSenseIdFromPredVarIndex.length; i += 2) {
        if (out_arr_GetSenseIdFromPredVarIndex.si[i] == nounEnd)
            return 0;
    }
    int doesLenExceedIndex = out_arr_GetSenseIdFromPredVarIndex.length > in_i_GetSenseIdFromPredVarIndex * 2;
    if (doesLenExceedIndex && nounEnd > out_arr_GetSenseIdFromPredVarIndex.si[out_iOfGreatest_GetSenseIdFromPredVarIndex])
        return 0;
    int retval;
    sqlite3_clear_bindings(segExistInVarSelectSense);
    sqlite3_reset(segExistInVarSelectSense);
    retval = sqlite3_bind_int64(segExistInVarSelectSense, 1, nounEnd);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(segExistInVarSelectSense);
    if (retval == SQLITE_ROW) {
        if (doesLenExceedIndex) {
            out_arr_GetSenseIdFromPredVarIndex.si[out_iOfGreatest_GetSenseIdFromPredVarIndex] = nounEnd;
            out_arr_GetSenseIdFromPredVarIndex.si[out_iOfGreatest_GetSenseIdFromPredVarIndex + 1] = sqlite3_column_int64(segExistInVarSelectSense, 0);
            for (size_t i = 0; i < out_arr_GetSenseIdFromPredVarIndex.length; i += 2) {
                if (out_arr_GetSenseIdFromPredVarIndex.si[i] > out_arr_GetSenseIdFromPredVarIndex.si[out_iOfGreatest_GetSenseIdFromPredVarIndex])
                    out_iOfGreatest_GetSenseIdFromPredVarIndex = i;
            }
        } else {
            if (out_arr_GetSenseIdFromPredVarIndex.length == 0 || nounEnd > out_arr_GetSenseIdFromPredVarIndex.si[out_iOfGreatest_GetSenseIdFromPredVarIndex])
                out_iOfGreatest_GetSenseIdFromPredVarIndex = out_arr_GetSenseIdFromPredVarIndex.length;
            SiDarrAddMem(nounEnd, &out_arr_GetSenseIdFromPredVarIndex);
            SiDarrAddMem(sqlite3_column_int64(segExistInVarSelectSense, 0), &out_arr_GetSenseIdFromPredVarIndex);
        }
    }
    return 0;
}

static sqlite3_int64 GetSenseIdFromPredVarIndex(const unsigned char *tabName, size_t tabNameLen, size_t i) {
    in_i_GetSenseIdFromPredVarIndex = i;
    out_arr_GetSenseIdFromPredVarIndex.capacity = 6;
    out_arr_GetSenseIdFromPredVarIndex.length = 0;
    out_arr_GetSenseIdFromPredVarIndex.si = malloc(6 * (sizeof *out_arr_GetSenseIdFromPredVarIndex.si));
    IterateAllNounEndsUnderPred(tabName, tabNameLen, Callback_GetSenseIdFromPredVarIndex);
    sqlite3_int64 senseId = out_arr_GetSenseIdFromPredVarIndex.si[out_iOfGreatest_GetSenseIdFromPredVarIndex + 1];
    free(out_arr_GetSenseIdFromPredVarIndex.si);
    return senseId;
}

static int GetPredTabNameFromVerbSegId(sqlite3_int64 segId, const unsigned char **ppcuc, size_t *plen) {
    int retval;
    sqlite3_clear_bindings(selectTabNameWhereVerbSegID);
    sqlite3_reset(selectTabNameWhereVerbSegID);
    retval = sqlite3_bind_int64(selectTabNameWhereVerbSegID, 1, segId);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectTabNameWhereVerbSegID);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        *ppcuc = sqlite3_column_blob(selectTabNameWhereVerbSegID, 0);
        *plen = sqlite3_column_bytes(selectTabNameWhereVerbSegID, 0);
        return 0;
    }

    sqlite3_clear_bindings(selectParentSegIdFromNounasWhereVerbSegId);
    sqlite3_reset(selectParentSegIdFromNounasWhereVerbSegId);
    retval = sqlite3_bind_int64(selectParentSegIdFromNounasWhereVerbSegId, 1, segId);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectParentSegIdFromNounasWhereVerbSegId);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        return GetPredTabNameFromNounSegId(sqlite3_column_int64(selectParentSegIdFromNounasWhereVerbSegId, 0), ppcuc, plen);
    }

    sqlite3_clear_bindings(selectParentSegIdFromGerundWhereVerbSegId);
    sqlite3_reset(selectParentSegIdFromGerundWhereVerbSegId);
    retval = sqlite3_bind_int64(selectParentSegIdFromGerundWhereVerbSegId, 1, segId);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectParentSegIdFromGerundWhereVerbSegId);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        return GetPredTabNameFromVerbSegId(sqlite3_column_int64(selectParentSegIdFromGerundWhereVerbSegId, 0), ppcuc, plen);
    }
    //todo: INVALID!!!
}

static int GetPredTabNameFromNounSegId(sqlite3_int64 segId, const unsigned char **ppcuc, size_t *plen) {
    int retval;
    sqlite3_clear_bindings(selectTabNameWhereNounSegID);
    sqlite3_reset(selectTabNameWhereNounSegID);
    retval = sqlite3_bind_int64(selectTabNameWhereNounSegID, 1, segId);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_bind_int64(selectTabNameWhereNounSegID, 2, segId);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectTabNameWhereNounSegID);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        *ppcuc = sqlite3_column_blob(selectTabNameWhereNounSegID, 0);
        *plen = sqlite3_column_bytes(selectTabNameWhereNounSegID, 0);
        return 0;
    }

    sqlite3_clear_bindings(selectParentSegIdFromNounasWhereNounSegId);
    sqlite3_reset(selectParentSegIdFromNounasWhereNounSegId);
    retval = sqlite3_bind_int64(selectParentSegIdFromNounasWhereNounSegId, 1, segId);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectParentSegIdFromNounasWhereNounSegId);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        return GetPredTabNameFromNounSegId(sqlite3_column_int64(selectParentSegIdFromNounasWhereNounSegId, 0), ppcuc, plen);
    }

    sqlite3_clear_bindings(selectParentSegIdFromGerundWhereNounSegId);
    sqlite3_reset(selectParentSegIdFromGerundWhereNounSegId);
    retval = sqlite3_bind_int64(selectParentSegIdFromGerundWhereNounSegId, 1, segId);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectParentSegIdFromGerundWhereNounSegId);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        return GetPredTabNameFromVerbSegId(sqlite3_column_int64(selectParentSegIdFromGerundWhereNounSegId, 0), ppcuc, plen);
    }
    //todo: INVALID!!!
}

static int Callback_GetNumOfVarLessThanSegUnderPred(sqlite3_int64 nounEnd) {
    int retval;
    if (nounEnd >= in_seg_GetNumOfVarLessThanSegUnderPred)
        return 0;
    for (size_t i = 0; i < out_arr_GetNumOfVarLessThanSegUnderPred.length; i++) {
        if (out_arr_GetNumOfVarLessThanSegUnderPred.si[i] == nounEnd)
            return 0;
    }
    sqlite3_clear_bindings(segExistInVar);
    sqlite3_reset(segExistInVar);
    retval = sqlite3_bind_int64(segExistInVar, 1, nounEnd);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(segExistInVar);
    //todo: check return value
    if (retval == SQLITE_ROW)
        SiDarrAddMem(nounEnd, &out_arr_GetNumOfVarLessThanSegUnderPred);
    return 0;
}

size_t GetNumOfVarLessThanSegUnderPred(const unsigned char *tabName, size_t tabNameLen, sqlite3_int64 segId) {
    in_seg_GetNumOfVarLessThanSegUnderPred = segId;
    out_arr_GetNumOfVarLessThanSegUnderPred.capacity = 6;
    out_arr_GetNumOfVarLessThanSegUnderPred.length = 0;
    out_arr_GetNumOfVarLessThanSegUnderPred.si = malloc(6 * (sizeof *out_arr_GetNumOfVarLessThanSegUnderPred.si));
    IterateAllNounEndsUnderPred(tabName, tabNameLen, Callback_GetNumOfVarLessThanSegUnderPred);
    free(out_arr_GetNumOfVarLessThanSegUnderPred.si);
    return out_arr_GetNumOfVarLessThanSegUnderPred.length;
}

sqlite3_int64 GetMaxIdOfSNLSense(sqlite3_int64 senseid) {
    sqlite3_int64 maxId = 0;
    int retval;
    sqlite3_clear_bindings(selectGreatestInsID);
    sqlite3_reset(selectGreatestInsID);
    retval = sqlite3_bind_int64(selectGreatestInsID, 1, senseid);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    retval = sqlite3_step(selectGreatestInsID);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        maxId = sqlite3_column_int64(selectGreatestInsID, 0);
    }

    sqlite3_clear_bindings(selectSegIDsWhereSenseID);
    sqlite3_reset(selectSegIDsWhereSenseID);
    retval = sqlite3_bind_int64(selectSegIDsWhereSenseID, 1, senseid);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    for (;;) {
        retval = sqlite3_step(selectSegIDsWhereSenseID);
        //todo: check return value
        if (retval != SQLITE_ROW)
            return maxId;
        sqlite3_int64 segID = sqlite3_column_int64(selectSegIDsWhereSenseID, 0);
        size_t tabNameLen;
        const unsigned char *tabName;
        GetPredTabNameFromNounSegId(segID, &tabName, &tabNameLen);
        //todo: check return value
        unsigned char selectStmtUC[16 + 3 + sizeof"SELECT " " FROM " " ORDER BY 1 DESC LIMIT 1"] = "SELECT ";
        size_t paramNameLen;
        ConvParamIndexToParamName(GetNumOfVarLessThanSegUnderPred(tabName, tabNameLen, segID), selectStmtUC + sizeof"SELECT " - 1, &paramNameLen);
        strcpy((char*)(selectStmtUC + sizeof"SELECT " - 1 + paramNameLen), " FROM ");
        strncat((char*)selectStmtUC, (char*)tabName, tabNameLen);
        strcat((char*)selectStmtUC, " ORDER BY 1 DESC LIMIT 1");
        sqlite3_stmt *selectStmt;
        retval = sqlite3_prepare_v2(databaseA, (char*)selectStmtUC, -1, &selectStmt, NULL);
        if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
        retval = sqlite3_step(selectStmt);
        //todo: check return value
        if (retval == SQLITE_ROW) {
            sqlite3_int64 maxInPred = sqlite3_column_int64(selectStmt, 0);
            if (maxInPred > maxId)
                maxId = maxInPred;
        }
        sqlite3_finalize(selectStmt);
    }
}

sqlite3_int64 GetMaxSNLSenseId() {
    sqlite3_reset(selectGreatestSNLSenseId);
    int retval = sqlite3_step(selectGreatestSNLSenseId);
    //todo: check return value
    if (retval == SQLITE_ROW)
        return sqlite3_column_int64(selectGreatestSNLSenseId, 0);
    else
        return 0;
}

int InsertIntoISNLSenseTab() {//unsigned char *data, size_t size) {
    int retval;
    size_t size;
    sqlite3_int64 newSenseBuf = 0;
    recvUntilBytesPtr(recvbuf, 1);
    switch (recvbuf[0]) {
            //todo: check any sense id must have appeared in synonym/inverseverb
            //todo: or, you add new ID
#define INSERT_IB(stmt)\
sqlite3_clear_bindings(stmt);\
sqlite3_reset(stmt);\
recvUntilBytesPtr(recvbuf, sizeof(sqlite3_int64));\
retval = sqlite3_bind_int64(stmt, 1, derefUcGetSi( recvbuf));\
if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
recvUntilBytesPtr(recvbuf, 2);\
size = recvbuf[0]+recvbuf[1]*256;\
if(size){\
    recvUntilBytesPtr(recvbuf, size);\
    retval = sqlite3_bind_blob(stmt, 2, recvbuf, size, SQLITE_TRANSIENT);\
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
} else{\
    retval = sqlite3_bind_zeroblob(stmt, 2, -1);\
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
}
#define INSERT_II(stmt)\
sqlite3_clear_bindings(stmt);\
sqlite3_reset(stmt);\
recvUntilBytesPtr(recvbuf, sizeof(sqlite3_int64));\
retval = sqlite3_bind_int64(stmt, 1, derefUcGetSi( recvbuf));\
if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
recvUntilBytesPtr(recvbuf, sizeof(sqlite3_int64));\
retval = sqlite3_bind_int64(stmt, 2, derefUcGetSi( recvbuf));\
if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
#define INSERT_III(stmt)\
INSERT_II(stmt);\
recvUntilBytesPtr(recvbuf, sizeof(sqlite3_int64));\
retval = sqlite3_bind_int64(stmt, 3, derefUcGetSi( recvbuf));\
if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
#define INSERT_IIII(stmt)\
INSERT_III(stmt);\
recvUntilBytesPtr(recvbuf, sizeof(sqlite3_int64));\
retval = sqlite3_bind_int64(stmt, 4, derefUcGetSi( recvbuf));\
if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
#define INSERT_BI(stmt)\
sqlite3_clear_bindings(stmt);\
sqlite3_reset(stmt);\
recvUntilBytesPtr(recvbuf, 1);\
if(recvbuf[0]){\
    newSenseBuf = GetMaxSNLSenseId()+1;\
    retval = sqlite3_bind_int64(stmt, 2, newSenseBuf);\
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
} else {\
    recvUntilBytesPtr(recvbuf, sizeof(sqlite3_int64));\
    retval = sqlite3_bind_int64(stmt, 2, derefUcGetSi( recvbuf));\
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
}\
recvUntilBytesPtr(recvbuf, 2);\
size = recvbuf[0]+recvbuf[1]*256;\
if(size){\
    recvUntilBytesPtr(recvbuf, size);\
    retval = sqlite3_bind_blob(stmt, 1, recvbuf, size, SQLITE_TRANSIENT);\
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
} else{\
    retval = sqlite3_bind_zeroblob(stmt, 1, -1);\
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
}
#define SEND_RES_MSG \
if(retval==SQLITE_DONE)\
    sendUntilBytesPtr((unsigned char *)"\x00", 1);\
else\
    sendUntilBytesPtr((unsigned char *)"\x01", 1)
        case 0://todo: INVALID?
            break;
        case 1:
            INSERT_IB(insertIntoInvertiblev);
            retval = sqlite3_step(insertIntoInvertiblev);
            SEND_RES_MSG;
            break;
        case 2:
            INSERT_IB(insertIntoNoninvertib);
            retval = sqlite3_step(insertIntoNoninvertib);
            SEND_RES_MSG;
            break;
        case 3:
            INSERT_IB(insertIntoBytnoun);
            retval = sqlite3_step(insertIntoBytnoun);
            SEND_RES_MSG;
            break;
        case 4:
            INSERT_IB(insertIntoDignoun);
            retval = sqlite3_step(insertIntoDignoun);
            SEND_RES_MSG;
            break;
        case 5:
            INSERT_IB(insertIntoConroot);
            retval = sqlite3_step(insertIntoConroot);
            SEND_RES_MSG;
            break;
        case 6:
            INSERT_IB(insertIntoAbsroot);
            retval = sqlite3_step(insertIntoAbsroot);
            SEND_RES_MSG;
            break;
        case 7:
            INSERT_II(insertIntoMutexcl2);
            retval = sqlite3_step(insertIntoMutexcl2);
            SEND_RES_MSG;
            break;
        case 8:
            INSERT_III(insertIntoMutexcl3);
            retval = sqlite3_step(insertIntoMutexcl3);
            SEND_RES_MSG;
            break;
        case 9:
            INSERT_II(insertIntoSubcate1);
            retval = sqlite3_step(insertIntoSubcate1);
            SEND_RES_MSG;
            break;
        case 10:
            INSERT_III(insertIntoSubcate2);
            retval = sqlite3_step(insertIntoSubcate2);
            SEND_RES_MSG;
            break;
        case 11:
            INSERT_III(insertIntoInter2);
            retval = sqlite3_step(insertIntoInter2);
            SEND_RES_MSG;
            break;
        case 12:
            INSERT_III(insertIntoUnion2);
            retval = sqlite3_step(insertIntoUnion2);
            SEND_RES_MSG;
            break;
        case 13:
            INSERT_IIII(insertIntoInter3);
            retval = sqlite3_step(insertIntoInter3);
            SEND_RES_MSG;
            break;
        case 14:
            INSERT_IIII(insertIntoUnion3);
            retval = sqlite3_step(insertIntoUnion3);
            SEND_RES_MSG;
            break;
        case 15:
            INSERT_III(insertIntoRelacomp2);
            retval = sqlite3_step(insertIntoRelacomp2);
            SEND_RES_MSG;
            break;
        case 16:
            INSERT_IIII(insertIntoRelacomp3);
            retval = sqlite3_step(insertIntoRelacomp3);
            SEND_RES_MSG;
            break;
        case 17:
            INSERT_BI(insertIntoSynonym);
            retval = sqlite3_step(insertIntoSynonym);
            if (newSenseBuf) {
                if (retval == SQLITE_DONE)
                    sendUntilBytesPtr((unsigned char *) &newSenseBuf, sizeof (sqlite3_int64));
                else
                    sendUntilBytesPtr((unsigned char *) "\0\0\0\0\0\0\0", 8);
            } else {
                SEND_RES_MSG;
            }
            break;
        case 18:
            INSERT_BI(insertIntoInverseverb);
            retval = sqlite3_step(insertIntoInverseverb);
            if (newSenseBuf) {
                if (retval == SQLITE_DONE)
                    sendUntilBytesPtr((unsigned char *) &newSenseBuf, sizeof (sqlite3_int64));
                else
                    sendUntilBytesPtr((unsigned char *) "\0\0\0\0\0\0\0", 8);
            } else {
                SEND_RES_MSG;
            }
            break;
        default://todo: INVALID?
            break;
#undef INSERT_IB
#undef INSERT_II
#undef INSERT_III
#undef INSERT_IIII
#undef INSERT_BI
    }
}

int CreatePredicateExplicitly() {
    //todo: check validity, including number of columns <= 999
    //todo: each seg can only appear once in those tables except for gerundasobj/sub,nounasobjec/subje,indeppr,indepcl

    //todo: check possible duplicate predicate

    int retval;
    unsigned char tabName[17];
    size_t tabNameLen;
    sqlite3_reset(selectGreatestTabNameFromIndepcl);
    retval = sqlite3_step(selectGreatestTabNameFromIndepcl);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        tabNameLen = sqlite3_column_bytes(selectGreatestTabNameFromIndepcl, 0);
        tabName[tabNameLen] = '0';
        memcpy(tabName, sqlite3_column_blob(selectGreatestTabNameFromIndepcl, 0), tabNameLen);
        SNLColNameIncrement(tabName, &tabNameLen);
        if (tabNameLen == 17)
            return 1;
    } else if (retval == SQLITE_DONE) {
        tabName[0] = 'a';
        tabNameLen = 1;
    }

    unsigned char *createStmtUCh = malloc(16000); //999*(12+4) == 15984, 16000 should be enough
    strcpy((char *) createStmtUCh, "CREATE TABLE ");
    strncat((char *) createStmtUCh, (char *) tabName, tabNameLen);
    strcat((char *) createStmtUCh, "(");
    size_t numOfCols = 0;
    unsigned char colName[] = {'a', '0', '0'};
    size_t colNameLen = 1;

    sqlite3_int64 greatestVerbSegId = 0;
    sqlite3_reset(selectGreatestVerbSegID);
    retval = sqlite3_step(selectGreatestVerbSegID);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        greatestVerbSegId = sqlite3_column_int64(selectGreatestVerbSegID, 0);
    }
    sqlite3_int64 greatestNounSegId = 0;
    sqlite3_reset(selectGreatestNounSegID);
    retval = sqlite3_step(selectGreatestNounSegID);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        greatestNounSegId = sqlite3_column_int64(selectGreatestNounSegID, 0);
    }
    struct SiDarr seqOfVerbSeg;
    seqOfVerbSeg.capacity = 5;
    seqOfVerbSeg.length = 0;
    seqOfVerbSeg.si = malloc(seqOfVerbSeg.capacity * (sizeof *seqOfVerbSeg.si));
    struct SiDarr seqOfNounSeg;
    seqOfNounSeg.capacity = 5;
    seqOfNounSeg.length = 0;
    seqOfNounSeg.si = malloc(seqOfNounSeg.capacity * (sizeof *seqOfNounSeg.si));
    size_t aforementionedInd;
    //unsigned char *originalPred = pred;
    //while (originalPred + size > pred) {
    sqlite3_int64 idbuf;
    int blen;
    for (;;) {
        recvUntilBytesPtr(recvbuf, 1);
        switch (recvbuf[0]) {
#define STRCATCREATETAB_INCRECOUNT(type)\
strncat((char *) createStmtUCh,(char *) colName,colNameLen);\
strcat((char *) createStmtUCh," " #type ",");\
SNLColNameIncrement(colName,&colNameLen);\
numOfCols++
#define PRED_PRRAM_BIND_SEGID(Xxxx,sqliteStmt,bindNum)\
recvUntilBytesPtr(recvbuf, sizeof(sqlite3_int64));\
aforementionedInd = SiDarrIndexOf(derefUcGetSi(recvbuf),&seqOf##Xxxx##Seg);\
if(aforementionedInd==seqOf##Xxxx##Seg.length){\
    greatest##Xxxx##SegId++;\
    retval = sqlite3_bind_int64(sqliteStmt, bindNum, greatest##Xxxx##SegId);\
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
    SiDarrAddMem(derefUcGetSi(recvbuf),&seqOf##Xxxx##Seg);\
} else {\
    retval = sqlite3_bind_int64(sqliteStmt, bindNum, greatest##Xxxx##SegId-(seqOf##Xxxx##Seg.length-1-aforementionedInd));\
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);\
}
            case 255:
                goto end_of_inserts;
            case 1://todo: INVALID!!
                break;
            case 2:
                //fixme: actually making new seg should be forbidden here
                //the 5 kinds of clauses must come after all segs are already prepared
                //any clause coming before seg is invalid
                //Otherwise the order of columns in Create Statement can be different from the Segments ID's order
                sqlite3_clear_bindings(insertIntoIndepcl);
                sqlite3_reset(insertIntoIndepcl);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoIndepcl, 1);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoIndepcl, 2);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoIndepcl, 3);
                retval = sqlite3_bind_blob(insertIntoIndepcl, 4, tabName, tabNameLen, SQLITE_TRANSIENT);
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                retval = sqlite3_step(insertIntoIndepcl);
                //todo: check return value
                break;
            case 3:
                sqlite3_clear_bindings(insertIntoConnounvar);
                sqlite3_reset(insertIntoConnounvar);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoConnounvar, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoConnounvar, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                retval = sqlite3_step(insertIntoConnounvar);
                //todo: check return value
                STRCATCREATETAB_INCRECOUNT(INTEGER);
                break;
            case 4:
                sqlite3_clear_bindings(insertIntoConnounins);
                sqlite3_reset(insertIntoConnounins);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoConnounins, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                idbuf = derefUcGetSi( recvbuf);
                retval = sqlite3_bind_int64(insertIntoConnounins, 2, idbuf);
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                recvUntilBytesPtr(recvbuf, 1);
                if (recvbuf[0]) {
                    retval = sqlite3_bind_int64(insertIntoConnounins, 3, GetMaxIdOfSNLSense(idbuf) + 1);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                } else {
                    recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                    retval = sqlite3_bind_int64(insertIntoConnounins, 3, derefUcGetSi( recvbuf));
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                }
                retval = sqlite3_step(insertIntoConnounins);
                //todo: check return value
                break;
            case 5:
                sqlite3_clear_bindings(insertIntoBytnounvar);
                sqlite3_reset(insertIntoBytnounvar);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoBytnounvar, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoBytnounvar, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                retval = sqlite3_step(insertIntoBytnounvar);
                //todo: check return value
                STRCATCREATETAB_INCRECOUNT(BLOB);
                break;
            case 6:
                sqlite3_clear_bindings(insertIntoBytnounins);
                sqlite3_reset(insertIntoBytnounins);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoBytnounins, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoBytnounins, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                recvUntilBytesPtr(recvbuf, 2);
                blen = recvbuf[1]*256 + recvbuf[0];
                if (blen == 0) {
                    retval = sqlite3_bind_zeroblob(insertIntoBytnounins, 3, -1);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                } else {
                    recvUntilBytesPtr(recvbuf, blen);
                    retval = sqlite3_bind_blob(insertIntoBytnounins, 3, recvbuf, blen, SQLITE_TRANSIENT);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                }
                retval = sqlite3_step(insertIntoBytnounins);
                //todo: check return value
                break;
            case 7:
                sqlite3_clear_bindings(insertIntoDignounvar);
                sqlite3_reset(insertIntoDignounvar);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoDignounvar, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoDignounvar, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                retval = sqlite3_step(insertIntoDignounvar);
                //todo: check return value
                STRCATCREATETAB_INCRECOUNT(BLOB);
                break;
            case 8:
                sqlite3_clear_bindings(insertIntoDignounins);
                sqlite3_reset(insertIntoDignounins);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoDignounins, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoDignounins, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                recvUntilBytesPtr(recvbuf, 2);
                blen = recvbuf[1]*256 + recvbuf[0];
                if (blen == 0) {
                    retval = sqlite3_bind_zeroblob(insertIntoDignounins, 3, -1);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                } else {
                    recvUntilBytesPtr(recvbuf, blen);
                    retval = sqlite3_bind_blob(insertIntoDignounins, 3, recvbuf, blen, SQLITE_TRANSIENT);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                }
                retval = sqlite3_step(insertIntoDignounins);
                //todo: check return value
                break;
            case 9:
                sqlite3_clear_bindings(insertIntoRawbytevar);
                sqlite3_reset(insertIntoRawbytevar);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoRawbytevar, 1);
                retval = sqlite3_step(insertIntoRawbytevar);
                //todo: check return value
                STRCATCREATETAB_INCRECOUNT(BLOB);
                break;
            case 10:
                sqlite3_clear_bindings(insertIntoRawbyteins);
                sqlite3_reset(insertIntoRawbyteins);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoRawbyteins, 1);
                recvUntilBytesPtr(recvbuf, 2);
                blen = recvbuf[1]*256 + recvbuf[0];
                if (blen == 0) {
                    retval = sqlite3_bind_zeroblob(insertIntoRawbyteins, 2, -1);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                } else {
                    recvUntilBytesPtr(recvbuf, blen);
                    retval = sqlite3_bind_blob(insertIntoRawbyteins, 2, recvbuf, blen, SQLITE_TRANSIENT);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                }
                retval = sqlite3_step(insertIntoRawbyteins);
                //todo: check return value
                break;
            case 11:
                sqlite3_clear_bindings(insertIntoRawdigivar);
                sqlite3_reset(insertIntoRawdigivar);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoRawdigivar, 1);
                retval = sqlite3_step(insertIntoRawdigivar);
                //todo: check return value
                STRCATCREATETAB_INCRECOUNT(BLOB);
                break;
            case 12:
                sqlite3_clear_bindings(insertIntoRawdigiins);
                sqlite3_reset(insertIntoRawdigiins);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoRawdigiins, 1);
                recvUntilBytesPtr(recvbuf, 2);
                blen = recvbuf[1]*256 + recvbuf[0];
                if (blen == 0) {
                    retval = sqlite3_bind_zeroblob(insertIntoRawdigiins, 2, -1);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                } else {
                    recvUntilBytesPtr(recvbuf, blen);
                    retval = sqlite3_bind_blob(insertIntoRawdigiins, 2, recvbuf, blen, SQLITE_TRANSIENT);
                    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                }
                retval = sqlite3_step(insertIntoRawdigiins);
                //todo: check return value
                break;
            case 13:
                sqlite3_clear_bindings(insertIntoNormalverb);
                sqlite3_reset(insertIntoNormalverb);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoNormalverb, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoNormalverb, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                retval = sqlite3_step(insertIntoNormalverb);
                //todo: check return value
                break;
            case 14:
                sqlite3_clear_bindings(insertIntoNormalnoun);
                sqlite3_reset(insertIntoNormalnoun);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoNormalnoun, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoNormalnoun, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                retval = sqlite3_step(insertIntoNormalnoun);
                //todo: check return value
                break;
            case 15:
                sqlite3_clear_bindings(insertIntoAdverbialcl);
                sqlite3_reset(insertIntoAdverbialcl);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoAdverbialcl, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoAdverbialcl, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                retval = sqlite3_step(insertIntoAdverbialcl);
                //todo: check return value
                break;
            case 16:
                sqlite3_clear_bindings(insertIntoRelativecla);
                sqlite3_reset(insertIntoRelativecla);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoRelativecla, 1);
                recvUntilBytesPtr(recvbuf, sizeof (sqlite3_int64));
                retval = sqlite3_bind_int64(insertIntoRelativecla, 2, derefUcGetSi( recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                retval = sqlite3_step(insertIntoRelativecla);
                //todo: check return value
                break;
            case 17:
                sqlite3_clear_bindings(insertIntoGerundasobj);
                sqlite3_reset(insertIntoGerundasobj);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoGerundasobj, 1);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoGerundasobj, 2);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoGerundasobj, 3);
                retval = sqlite3_step(insertIntoGerundasobj);
                //todo: check return value
                break;
            case 18:
                sqlite3_clear_bindings(insertIntoGerundassub);
                sqlite3_reset(insertIntoGerundassub);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoGerundassub, 1);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoGerundassub, 2);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoGerundassub, 3);
                retval = sqlite3_step(insertIntoGerundassub);
                //todo: check return value
                break;
            case 19:
                sqlite3_clear_bindings(insertIntoNounasobjec);
                sqlite3_reset(insertIntoNounasobjec);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoNounasobjec, 1);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoNounasobjec, 2);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoNounasobjec, 3);
                retval = sqlite3_step(insertIntoNounasobjec);
                //todo: check return value
                break;
            case 20:
                sqlite3_clear_bindings(insertIntoNounassubje);
                sqlite3_reset(insertIntoNounassubje);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoNounassubje, 1);
                PRED_PRRAM_BIND_SEGID(Verb, insertIntoNounassubje, 2);
                PRED_PRRAM_BIND_SEGID(Noun, insertIntoNounassubje, 3);
                retval = sqlite3_step(insertIntoNounassubje);
                //todo: check return value
                break;
            default://todo: INVALID!!
                break;
#undef PRED_PRRAM_BIND_SEGID
#undef STRCATCREATETAB_INCRECOUNT
        }
    }
end_of_inserts:

    strcat((char *) createStmtUCh, "PRIMARY KEY(");
    colName[0] = 'a';
    colName[1] = '0';
    colName[2] = '0';
    colNameLen = 1;
    for (size_t i = 1; i <= numOfCols; i++) {
        strncat((char *) createStmtUCh, (char *) colName, colNameLen);
        strcat((char *) createStmtUCh, ",");
        SNLColNameIncrement(colName, &colNameLen);
    }
    createStmtUCh[strlen((char *) createStmtUCh) - 1] = ')';
    strcat((char *) createStmtUCh, ")WITHOUT ROWID");
    
    FinalizeSqlite3Stmts();//note this seems to be a sqlite BUG, it gives a false error (17) message for schema changed and then sqlite3_step is called. For avoiding this ugly message (and probable performance hit), all statements are re-prepared.

    char *zErrMsg = NULL;
    sqlite3_exec(databaseA, (char *) createStmtUCh, NULL, NULL, &zErrMsg);
    //todo: check
    if (zErrMsg != NULL) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    
    PrepareSqlite3Stmts();

    free(createStmtUCh);
    free(seqOfNounSeg.si);
    free(seqOfVerbSeg.si);
}

//todo: preview the impact first?

int DropPredicateTabExplicity(unsigned char *tabName, size_t size) {
    return 1;
}

//todo: preview the impact first?

int DeletePropFromPred(unsigned char *criteria, size_t size) {
    return 1;
}

int InsertPropIntoPred() {
    int retval;
    int tabnamelen;
    unsigned char *bitArr = malloc(999 / CHAR_BIT + (999 % CHAR_BIT != 0));
    size_t numOfColumns = 0;
    unsigned char *sqlstmt = malloc(2100); //2*999= 1998, 2100 should be enough
    strcpy((char *) sqlstmt, "INSERT INTO ");
    recvUntilBytesPtr(recvbuf, 2);
    tabnamelen = recvbuf[0] + recvbuf[1]*256;
    recvUntilBytesPtr(recvbuf, tabnamelen);
    strncat((char *) sqlstmt, (char *) recvbuf, tabnamelen);
    strcat((char *) sqlstmt, " VALUES(");
    unsigned char tableInforStmtCh[36] = "PRAGMA TABLE_INFO("; //18+16+1+1==36
    strncat((char *) tableInforStmtCh, (char *) recvbuf, tabnamelen);
    strcat((char *) tableInforStmtCh, ")");
    sqlite3_stmt *tableInfo = NULL;
    retval = sqlite3_prepare_v2(databaseA, (char *) tableInforStmtCh, -1, &tableInfo, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    for (;;) {
        retval = sqlite3_step(tableInfo);
        //todo: check
        if (retval != SQLITE_ROW)
            break;
        strcat((char *) sqlstmt, "?,");
        if (memcmp(sqlite3_column_text(tableInfo, 2), "BLOB", 4))
            SetBitInUCArrTo1(bitArr, numOfColumns);
        else
            SetBitInUCArrTo0(bitArr, numOfColumns);
        numOfColumns++;
    }
    sqlstmt[strlen((char *) sqlstmt) - 1] = ')';
    sqlite3_finalize(tableInfo);
    sqlite3_stmt *insertstmt = NULL;
    retval = sqlite3_prepare_v2(databaseA, (char *) sqlstmt, -1, &insertstmt, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);

    for (size_t i = 0; i < numOfColumns; i++) {
        if (GetBitInUCArr(bitArr, i)) {
            recvUntilBytesPtr(recvbuf, 1);
            if (recvbuf[0]) {
                sqlite3_int64 int64buffer = GetMaxIdOfSNLSense(GetSenseIdFromPredVarIndex(tableInforStmtCh + sizeof "PRAGMA TABLE_INFO("-1, tabnamelen, i)) + 1;
                retval = sqlite3_bind_int64(insertstmt, i + 1, int64buffer);
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
                sendUntilBytesPtr((unsigned char *) &int64buffer, 8);
            } else {
                recvUntilBytesPtr(recvbuf, 8);
                retval = sqlite3_bind_int64(insertstmt, i + 1, derefUcGetSi(recvbuf));
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
            }
        } else {
            recvUntilBytesPtr(recvbuf, 2);
            int size = recvbuf[0] + recvbuf[1]*256;
            if (size) {
                recvUntilBytesPtr(recvbuf, size);
                retval = sqlite3_bind_blob(insertstmt, i + 1, recvbuf, size, SQLITE_TRANSIENT);
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
            } else {
                retval = sqlite3_bind_zeroblob(insertstmt, i + 1, -1);
                if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
            }
        }
    }
    retval = sqlite3_step(insertstmt);
    sqlite3_finalize(insertstmt);
    free(bitArr);
    free(sqlstmt);
}

int IIDatabaseIDConsumptionProgress() {
    //todo: if any ID/tablename is approaching the max value, return warning.
}

int IIDatabaseDeployment() {
    //    
    //    //THE SIZE OPTIMAL SCHEME!!!!!
    //    //todo: maybe determadj is unnecessary?? by default = ANY, [some_subset_or_ins of NOUNSENSE] = SOME NOUNSENSE
    //    //todo: or maybe you should add table someofnoun and someofverb, but this doesn't solve some problem like some NOUNSENSE with relative clause
    //    //sqlite3_exec(databaseA, "CREATE TABLE determadj(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE invertiblev(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE noninvertib(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE bytnoun(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE dignoun(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE conroot(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE absroot(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE mutexcl2(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE mutexcl3(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE subcate1(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE subcate2(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE inter2(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE union2(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE inter3(a INTEGER, b INTEGER, c INTEGER, d INTEGER, PRIMARY KEY(b,c,d))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE union3(a INTEGER, b INTEGER, c INTEGER, d INTEGER, PRIMARY KEY(b,c,d))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE relacomp2(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE relacomp3(a INTEGER, b INTEGER, c INTEGER, d INTEGER, PRIMARY KEY(b,c,d))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE synonym(a BLOB, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE inverseverb(a BLOB, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //
    //    //todo: shouldn't we have a indepclNEGATIVE???? for negation NOT. And the same for relative clause/adverbial clause
    //    //todo: or maybe you shouldn't add any table, just use DOESN'T to describe the verb, so just use adverbialcl
    //    sqlite3_exec(databaseA, "CREATE TABLE indeppr(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE indepcl(a INTEGER, b INTEGER, c INTEGER, d BLOB, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE connounvar(a INTEGER, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE connounins(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE bytnounvar(a INTEGER, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE bytnounins(a INTEGER, b INTEGER, c BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE dignounvar(a INTEGER, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE dignounins(a INTEGER, b INTEGER, c BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE rawbytevar(a INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE rawbyteins(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE rawdigivar(a INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE rawdigiins(a INTEGER, b BLOB, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE normalverb(a INTEGER, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE normalnoun(a INTEGER, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE adverbialcl(a INTEGER, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE relativecla(a INTEGER, b INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE gerundasobj(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE gerundassub(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE nounasobjec(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE nounassubje(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    




    //THE SELECTING OPTIMAL SCHEME!!!!!
    //    sqlite3_exec(databaseA, "CREATE TABLE invertiblev(a INTEGER, b BLOB, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE noninvertib(a INTEGER, b BLOB, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE bytnoun(a INTEGER, b BLOB, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE dignoun(a INTEGER, b BLOB, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE conroot(a INTEGER, b BLOB, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE absroot(a INTEGER, b BLOB, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE mutexcl2(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE mutexcl3(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE subcate1(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE subcate2(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE inter2(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE union2(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE inter3(a INTEGER, b INTEGER, c INTEGER, d INTEGER, PRIMARY KEY(a,b,c,d))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE union3(a INTEGER, b INTEGER, c INTEGER, d INTEGER, PRIMARY KEY(a,b,c,d))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE relacomp2(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE relacomp3(a INTEGER, b INTEGER, c INTEGER, d INTEGER, PRIMARY KEY(a,b,c,d))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE synonym(a BLOB, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE inverseverb(a BLOB, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //
    //    sqlite3_exec(databaseA, "CREATE TABLE indeppr(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE indepcl(a INTEGER, b INTEGER, c INTEGER, d BLOB, PRIMARY KEY(a,b,c,d))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE connounvar(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE connounins(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE bytnounvar(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE bytnounins(a INTEGER, b INTEGER, c BLOB, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE dignounvar(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE dignounins(a INTEGER, b INTEGER, c BLOB, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE rawbytevar(a INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE rawbyteins(a INTEGER, b BLOB, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE rawdigivar(a INTEGER, PRIMARY KEY(a))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE rawdigiins(a INTEGER, b BLOB, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE normalverb(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE normalnoun(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE adverbialcl(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE relativecla(a INTEGER, b INTEGER, PRIMARY KEY(a,b))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE gerundasobj(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE gerundassub(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE nounasobjec(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    //    sqlite3_exec(databaseA, "CREATE TABLE nounassubje(a INTEGER, b INTEGER, c INTEGER, PRIMARY KEY(a,b,c))WITHOUT ROWID", NULL, NULL, NULL);
    return 0;
}

int ExecQuerySelect() {
    //todo: check if it's a read only statement?
    int retval;
    int numOfBytes;
    recvUntilBytesPtr(recvbuf, 2);
    int size = recvbuf[0] + recvbuf[1]*256;
    recvUntilBytesPtr(recvbuf, size);
    sqlite3_stmt *selectStmt;
    retval = sqlite3_prepare_v2(databaseA, (char *) recvbuf, size, &selectStmt, NULL);
    if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);

    for (int paraInd = 1;; paraInd++) {
        recvUntilBytesPtr(recvbuf, 1);
        if (recvbuf[0])
            break;
        recvUntilBytesPtr(recvbuf, 2);
        size = recvbuf[0] + recvbuf[1]*256;
        recvUntilBytesPtr(recvbuf, size);
        retval = sqlite3_bind_blob(selectStmt, paraInd, recvbuf, size, SQLITE_TRANSIENT);
        if (retval != SQLITE_OK) printf("ERROR: %s,%s,%d. CODE: %d.\n", __FILE__, __func__, __LINE__, retval);
    }
    size = sqlite3_column_count(selectStmt);
    sqlite3_int64 int64buffer;
    for (;;) {
        retval = sqlite3_step(selectStmt);
        if (retval != SQLITE_ROW)
            break;
        sendUntilBytesPtr((unsigned char *) "\x00", 1);
        for (int iCol = 0; iCol < size; iCol++) {
            switch (sqlite3_column_type(selectStmt, iCol)) {
            case SQLITE_INTEGER:
                int64buffer = sqlite3_column_int64(selectStmt, iCol);
                sendUntilBytesPtr((unsigned char *) &int64buffer, 8);
                break;
            case SQLITE_BLOB:
                numOfBytes = sqlite3_column_bytes(selectStmt, iCol);
                sendUntilBytesPtr((unsigned char *) &numOfBytes, 2);
                sendUntilBytesPtr(sqlite3_column_blob(selectStmt, iCol), numOfBytes);
                break;
            default:
                //todo: error?
                break;
            }
        }
    }
    sendUntilBytesPtr((unsigned char *) "\x01", 1);
    sqlite3_finalize(selectStmt);
}

int analyzeRecvBuf(unsigned char recvc) {
    switch (recvc) {
        case 0:
            GetAllISNLSenseTableData();
            break;
        case 1:
            InsertIntoISNLSenseTab();
            break;
        case 2:
            CreatePredicateExplicitly();
            break;
        case 3:
            GetAllPredicatesData();
            break;
        case 4:
            ExecQuerySelect();
            break;
        case 5:
            InsertPropIntoPred();
            break;
    }
    return 0;
}
