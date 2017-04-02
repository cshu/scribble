

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "SNLTypes.h"
#include "SNL.h"



//todo: plan all the max length of data in various steps.

//todo: lock db files at start, also check all modified time and file size to find possible tamper.

sqlite3 *databaseA = NULL;
sqlite3_stmt *insertStmtForB;
sqlite3_stmt *insertDefaultPro;

//sqlite3_stmt *sqliteMasterSelEqTabName;
sqlite3_stmt *predicatesSelEqPred;
sqlite3_stmt *virtpredsSelEqPred;
sqlite3_stmt *virtpredsSelMaxIdEqTabn;
sqlite3_stmt *virtpredsIns;
sqlite3_stmt *synonymSelEqInfl;
sqlite3_stmt *subepitSelEqSubep;
sqlite3_stmt *epithetSelEqSpell;
sqlite3_stmt *dignounSelEqSpell;
sqlite3_stmt *bytnounSelEqSpell;
sqlite3_stmt *subnounSelEqSub;
sqlite3_stmt *abbrnounSelEqAbbr;
sqlite3_stmt *absidissSelEqSpell;
sqlite3_stmt *conidissSelEqSpell;
sqlite3_stmt *absrootSelEqSpell;
sqlite3_stmt *conrootSelEqSpell;
sqlite3_stmt *maxLenMaxPredTabName;
sqlite3_stmt *predicatesIns;

static int Sqlite3execcb_GetNumOfCol(void* ptr, int numOfCol, char** arrColText, char** arrColName) {
    *(int *) ptr = numOfCol;
    return 0;
}

static enum snlSpellLookupResult SearchForNounNotDigBytNoun(const unsigned char* ch, size_t size, int isConfirmedABBRNounOrSUBNoun) {
    sqlite3_clear_bindings(subnounSelEqSub);
    sqlite3_reset(subnounSelEqSub);
    sqlite3_bind_blob(subnounSelEqSub, 1, ch, size, SQLITE_TRANSIENT);
    int retval = sqlite3_step(subnounSelEqSub);
    //todo: check
    if (retval == SQLITE_ROW) {
        int numByt = sqlite3_column_bytes(subnounSelEqSub, 0);
        unsigned char *uch = malloc(numByt);
        memcpy(uch, sqlite3_column_blob(subnounSelEqSub, 0), numByt);
        numByt = SearchForNounNotDigBytNoun(uch, numByt, 2); //todo: notice that this noun inherit line may have went through both ABBR and SUB when it gets here
        free(uch);
        return numByt;
    }
    sqlite3_clear_bindings(abbrnounSelEqAbbr);
    sqlite3_reset(abbrnounSelEqAbbr);
    sqlite3_bind_blob(abbrnounSelEqAbbr, 1, ch, size, SQLITE_TRANSIENT);
    retval = sqlite3_step(abbrnounSelEqAbbr);
    //todo: check
    if (retval == SQLITE_ROW) {
        int numByt = sqlite3_column_bytes(abbrnounSelEqAbbr, 0);
        unsigned char *uch = malloc(numByt);
        memcpy(uch, sqlite3_column_blob(abbrnounSelEqAbbr, 0), numByt);
        numByt = SearchForNounNotDigBytNoun(uch, numByt, 1); //todo: notice that this noun inherit line may have went through both ABBR and SUB when it gets here
        free(uch);
        return numByt;
    }

    sqlite3_clear_bindings(absidissSelEqSpell);
    sqlite3_reset(absidissSelEqSpell);
    sqlite3_bind_blob(absidissSelEqSpell, 1, ch, size, SQLITE_TRANSIENT);
    retval = sqlite3_step(absidissSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW) {
        switch (isConfirmedABBRNounOrSUBNoun) {
            case 0:
                return snlSpellLookupResult_ABS;
                break;
            case 1:
                return snlSpellLookupResult_ABBRNOUN_OF_ABS;
                break;
            case 2:
                return snlSpellLookupResult_SUBNOUN_OF_ABS;
                break;
        }
    }

    sqlite3_clear_bindings(conidissSelEqSpell);
    sqlite3_reset(conidissSelEqSpell);
    sqlite3_bind_blob(conidissSelEqSpell, 1, ch, size, SQLITE_TRANSIENT);
    retval = sqlite3_step(conidissSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW) {
        switch (isConfirmedABBRNounOrSUBNoun) {
            case 0:
                return snlSpellLookupResult_CON;
                break;
            case 1:
                return snlSpellLookupResult_ABBRNOUN_OF_CON;
                break;
            case 2:
                return snlSpellLookupResult_SUBNOUN_OF_CON;
                break;
        }
    }
    sqlite3_clear_bindings(conrootSelEqSpell);
    sqlite3_reset(conrootSelEqSpell);
    sqlite3_bind_blob(conrootSelEqSpell, 1, ch, size, SQLITE_TRANSIENT);
    retval = sqlite3_step(conrootSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW) {
        switch (isConfirmedABBRNounOrSUBNoun) {
            case 0:
                return snlSpellLookupResult_CON;
                break;
            case 1:
                return snlSpellLookupResult_ABBRNOUN_OF_CON;
                break;
            case 2:
                return snlSpellLookupResult_SUBNOUN_OF_CON;
                break;
        }
    }
    sqlite3_clear_bindings(absrootSelEqSpell);
    sqlite3_reset(absrootSelEqSpell);
    sqlite3_bind_blob(absrootSelEqSpell, 1, ch, size, SQLITE_TRANSIENT);
    retval = sqlite3_step(absrootSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW) {
        switch (isConfirmedABBRNounOrSUBNoun) {
            case 0:
                return snlSpellLookupResult_ABS;
                break;
            case 1:
                return snlSpellLookupResult_ABBRNOUN_OF_ABS;
                break;
            case 2:
                return snlSpellLookupResult_SUBNOUN_OF_ABS;
                break;
        }
    } else if (retval == SQLITE_DONE) {
        return snlSpellLookupResult_NONEXISTENT;
    }
}

static enum snlSpellLookupResult SearchForEpithet(const unsigned char* ch, size_t size, int isConfirmedEpithet) {
    sqlite3_clear_bindings(subepitSelEqSubep);
    sqlite3_reset(subepitSelEqSubep);
    sqlite3_bind_blob(subepitSelEqSubep, 1, ch, size, SQLITE_TRANSIENT);
    int retval = sqlite3_step(subepitSelEqSubep);
    //todo: check
    if (retval == SQLITE_ROW) {
        return snlSpellLookupResult_SUBEPIT;
        //int numByt = sqlite3_column_bytes(subepitSelEqSubep, 0);
        //ch = malloc(numByt);
        //memcpy(ch, sqlite3_column_blob(subepitSelEqSubep, 0), numByt);
        //numByt = SearchForEpithet(ch,numByt,1);
        //free(ch);
        //return numByt;
    }

    sqlite3_clear_bindings(epithetSelEqSpell);
    sqlite3_reset(epithetSelEqSpell);
    sqlite3_bind_blob(epithetSelEqSpell, 1, ch, size, SQLITE_TRANSIENT);
    retval = sqlite3_step(epithetSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW) {
        return snlSpellLookupResult_EPITHET;
        //if(isConfirmedEpithet)
        //    return snlSpellLookupResult_SUBEPIT;
        //else
        //    return snlSpellLookupResult_EPITHET;
    } else if (retval == SQLITE_DONE) {
        return snlSpellLookupResult_NONEXISTENT; //todo: check IIDB integrity? must appear in epithet table, if(isConfirmedEpithet) IIDB corrupt
    }
}

/**
 * //todo: error code
 * @param ch
 * @param size
 * @return 
 */
int ConvToOrigIfIsInflec(const unsigned char **ch, size_t *size) {
    sqlite3_clear_bindings(synonymSelEqInfl);
    sqlite3_reset(synonymSelEqInfl);
    sqlite3_bind_blob(synonymSelEqInfl, 1, *ch, *size, SQLITE_TRANSIENT);
    int retval = sqlite3_step(synonymSelEqInfl);
    //todo: check
    if (retval == SQLITE_ROW) {
        *ch = sqlite3_column_blob(synonymSelEqInfl, 0);
        *size = sqlite3_column_bytes(synonymSelEqInfl, 0);
    }
    return 0;
}

int ConvToIdIssuerIfItIsnt(const unsigned char **ch, size_t *size) {
    
    sqlite3_clear_bindings(absidissSelEqSpell);
    sqlite3_reset(absidissSelEqSpell);
    sqlite3_bind_blob(absidissSelEqSpell, 1, *ch, *size, SQLITE_TRANSIENT);
    int retval = sqlite3_step(absidissSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW) {
        //*size = sqlite3_column_bytes(absidissSelEqSpell, 0);
        return 0;
    }
    sqlite3_clear_bindings(conidissSelEqSpell);
    sqlite3_reset(conidissSelEqSpell);
    sqlite3_bind_blob(conidissSelEqSpell, 1, *ch, *size, SQLITE_TRANSIENT);
    retval = sqlite3_step(conidissSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW) {
        return 0;
    }
}

static enum snlSpellLookupResult SNLSpellLookup(const unsigned char* ch, size_t size) {
    if (ch == NULL || size == 0)
        return snlSpellLookupResult_NONEXISTENT;
    if (size == 1) {
        if (ch[0] > '/' && ch[0] < ':')
            return snlSpellLookupResult_DIGITS;
    } else if (ch[0] > '0' && ch[0] < ':') {
        for (int i = 1;;) {
            if (ch[i] < '0' || ch[i] > '9') {
                break;
            }
            i++;
            if (i == size)
                return snlSpellLookupResult_DIGITS;
        }
    }

    int retval = ConvToOrigIfIsInflec(&ch,&size);
    int result = SearchForEpithet(ch, size, 0);
    if (result != snlSpellLookupResult_NONEXISTENT)
        return result;
    sqlite3_clear_bindings(dignounSelEqSpell);
    sqlite3_reset(dignounSelEqSpell);
    sqlite3_bind_blob(dignounSelEqSpell, 1, ch, size, SQLITE_TRANSIENT);
    retval = sqlite3_step(dignounSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW)
        return snlSpellLookupResult_DIGNOUN;
    sqlite3_clear_bindings(bytnounSelEqSpell);
    sqlite3_reset(bytnounSelEqSpell);
    sqlite3_bind_blob(bytnounSelEqSpell, 1, ch, size, SQLITE_TRANSIENT);
    retval = sqlite3_step(bytnounSelEqSpell);
    //todo: check
    if (retval == SQLITE_ROW)
        return snlSpellLookupResult_BYTNOUN;
    return SearchForNounNotDigBytNoun(ch, size, 0);
}



/**
 * //todo: proposition's size should not exceed C INT positive number upper limit.
 * @param proposition
 * @param rowId
 * @return 
 */
int InsertProposition(struct UCArr *proposition, sqlite3_int64 *rowId) {
    //todo: the proposition consists of 2 parts, 1st part is PREDICATE ending with a NUL, 2nd part is like this: loop(2 bytes+bytes OR 21 bytes literal digits), the 2 bytes are the num of bytes
    //todo: if 2 bytes is equal to 0x0000, bind NULL, then start the next loop. As for # literal digits, first byte of 0x00 means NULL.
    //todo: 2 bytes for the num of bytes (little endianness), but upper limit of length is NOT 65535. It's 50000 (SQLITE_MAX_LIKE_PATTERN_LENGTH for any BLOB in II), and lower limit is 1.
    //todo: # column binding uses ASCII 0-9, leading part is bytes 0|[1-9][0-9]{0,19}, trailing part is one or multiple NULLs, if first byte is not 0-9, it has special meaning.
    //todo: # column with first byte 0x01 means auto use new ID

    //todo: select from predicates and virtpreds to find the exact predicate, if not found, return failure.
    //todo: strlen is not safe to use
    size_t predlen = strlen(proposition->ch);
    if (predlen > 50000 || predlen == 0)//checking predlen for preventing attack
        return 1; //todo: return NOT VALID LENGTH of predicate
    unsigned char *insertStCh = malloc(2100); //2*998= 1996, 2100 should be enough
    strcpy(insertStCh, "INSERT INTO ");
    sqlite3_clear_bindings(predicatesSelEqPred);
    sqlite3_reset(predicatesSelEqPred);
    sqlite3_bind_blob(predicatesSelEqPred, 1, proposition->ch, predlen, SQLITE_TRANSIENT);
    int retval = sqlite3_step(predicatesSelEqPred);
    //todo: check return value and ...
    sqlite3_int64 vtn = 0;
    if (retval == SQLITE_ROW) {
        const unsigned char *tabName = sqlite3_column_blob(predicatesSelEqPred, 0);
        int tabNamLen = sqlite3_column_bytes(predicatesSelEqPred, 0);
        strncat(insertStCh, tabName, tabNamLen);
    } else if (retval == SQLITE_DONE) {
        sqlite3_clear_bindings(virtpredsSelEqPred);
        sqlite3_reset(virtpredsSelEqPred);
        sqlite3_bind_blob(virtpredsSelEqPred, 1, proposition->ch, predlen, SQLITE_TRANSIENT);
        int retval2 = sqlite3_step(virtpredsSelEqPred);
        //todo: check return value and ...
        if (retval2 == SQLITE_ROW) {
            const unsigned char *tabName2 = sqlite3_column_blob(virtpredsSelEqPred, 0);
            int tabNamLen2 = sqlite3_column_bytes(virtpredsSelEqPred, 0);
            vtn = sqlite3_column_int64(virtpredsSelEqPred, 1);
            strncat(insertStCh, tabName2, tabNamLen2);
        } else if (retval2 == SQLITE_DONE) {
            return 1; //todo: return NO PRED
        }
    }

    unsigned char *paramuc = proposition->ch + predlen + 1;
    int failCode = 0;
    sqlite3_stmt *insertSt = NULL;
    unsigned char *bindTypes = malloc(998);
    if (vtn) {
        unsigned short *bindIndices = malloc(999 * sizeof (unsigned short)); //0 terminated
        struct UCArr tmpPredicate = {proposition->ch, predlen};
        GetBindIndicesOfVirtPred(&tmpPredicate, bindIndices, bindTypes);
        strcat(insertStCh, " VALUES(?,");
        for (size_t i = 0; bindIndices[i]; i++) {
            strcat(insertStCh, "?,");
        }
        insertStCh[strlen(insertStCh) - 1] = ')';
        retval = sqlite3_prepare_v2(databaseA, insertStCh, -1, &insertSt, NULL);
        sqlite3_bind_int64(insertSt, 1, vtn);
        for (size_t i = 0; bindIndices[i]; i++) {
            if (paramuc + 2 > proposition->ch + proposition->size) {
                failCode = 1;
                break;
            }
            switch (bindTypes[i]) {
                case 0:
                    if (*paramuc == 0) {
                        failCode = 1;
                        break;
                    } else if (*paramuc == 1) {
                        //todo:bind new ID
                        struct UCArr noun = Get1stNounOfDigRelaClauParamAtIndex(proposition->ch, i);
                        ConvToOrigIfIsInflec((const unsigned char **)&noun.ch,&noun.size);
                        ConvToIdIssuerIfItIsnt((const unsigned char **)&noun.ch,&noun.size);
                    } else if (*paramuc > '/' && *paramuc < ':') {
                        sqlite3_bind_int64(insertSt, bindIndices[i], atoll(paramuc));
                    } else {
                        failCode = 1;
                        break;
                    }
                    paramuc += 21;
                    break;
                case 1:
                    if (*paramuc == 0) {
                        sqlite3_bind_null(insertSt, bindIndices[i]);
                    } else if (*paramuc == 1) {
                        //todo:bind new ID
                    } else if (*paramuc > '/' && *paramuc < ':') {
                        sqlite3_bind_int64(insertSt, bindIndices[i], atoll(paramuc));
                    } else {
                        failCode = 1;
                        break;
                    }
                    paramuc += 21;
                    break;
                case 2:
                    if (paramuc[0] == 0 && paramuc[1] == 0) {
                        failCode = 1;
                        break;
                    } else {
                        sqlite3_bind_blob(insertSt, bindIndices[i], paramuc + 2, paramuc[1]*256 + paramuc[0], SQLITE_TRANSIENT);
                    }
                    paramuc += 2 + paramuc[1]*256 + paramuc[0];
                    break;
                case 3:
                    if (paramuc[0] == 0 && paramuc[1] == 0) {
                        sqlite3_bind_null(insertSt, bindIndices[i]);
                    } else {
                        sqlite3_bind_blob(insertSt, bindIndices[i], paramuc + 2, paramuc[1]*256 + paramuc[0], SQLITE_TRANSIENT);
                    }
                    paramuc += 2 + paramuc[1]*256 + paramuc[0];
                    break;
            }
            if (failCode)
                break;
        }
        free(bindIndices);
    } else {
        unsigned char tableInforStmtCh[36] = "PRAGMA TABLE_INFO("; //18+16+1+1==36
        strcat(tableInforStmtCh, insertStCh + sizeof ("INSERT INTO ") - 1);
        strcat(tableInforStmtCh, ")");
        strcat(insertStCh, " VALUES(");
        sqlite3_stmt *tableInfo = NULL;
        retval = sqlite3_prepare_v2(databaseA, tableInforStmtCh, -1, &tableInfo, NULL);
        //todo: check
        size_t colCID = 0;
        for (;;) {
            retval = sqlite3_step(tableInfo);
            //todo: check return value
            if (retval == SQLITE_ROW) {
                strcat(insertStCh, "?,");
                if (memcmp(sqlite3_column_text(tableInfo, 2), "BLOB", 4)) {
                    if (sqlite3_column_int(tableInfo, 3))
                        bindTypes[colCID] = 0;
                    else
                        bindTypes[colCID] = 1;
                } else {
                    if (sqlite3_column_int(tableInfo, 3))
                        bindTypes[colCID] = 2;
                    else
                        bindTypes[colCID] = 3;
                }
                colCID++;
            } else {
                break;
            }
        }
        sqlite3_finalize(tableInfo);
        insertStCh[strlen(insertStCh) - 1] = ')';
        retval = sqlite3_prepare_v2(databaseA, insertStCh, -1, &insertSt, NULL);
        for (size_t i = 0; i < colCID; i++) {
            if (paramuc + 2 > proposition->ch + proposition->size) {
                failCode = 1;
                break;
            }
            switch (bindTypes[i]) {
                case 0:
                    if (*paramuc == 0) {
                        failCode = 1;
                        break;
                    } else if (*paramuc == 1) {
                        //todo:bind new ID
                    } else if (*paramuc > '/' && *paramuc < ':') {
                        sqlite3_bind_int64(insertSt, i + 1, atoll(paramuc));
                    } else {
                        failCode = 1;
                        break;
                    }
                    paramuc += 21;
                    break;
                case 1:
                    if (*paramuc == 0) {
                        sqlite3_bind_null(insertSt, i + 1);
                    } else if (*paramuc == 1) {
                        //todo:bind new ID
                    } else if (*paramuc > '/' && *paramuc < ':') {
                        sqlite3_bind_int64(insertSt, i + 1, atoll(paramuc));
                    } else {
                        failCode = 1;
                        break;
                    }
                    paramuc += 21;
                    break;
                case 2:
                    if (paramuc[0] == 0 && paramuc[1] == 0) {
                        failCode = 1;
                        break;
                    } else {
                        sqlite3_bind_blob(insertSt, i + 1, paramuc + 2, paramuc[1]*256 + paramuc[0], SQLITE_TRANSIENT);
                    }
                    paramuc += 2 + paramuc[1]*256 + paramuc[0];
                    break;
                case 3:
                    if (paramuc[0] == 0 && paramuc[1] == 0) {
                        sqlite3_bind_null(insertSt, i + 1);
                    } else {
                        sqlite3_bind_blob(insertSt, i + 1, paramuc + 2, paramuc[1]*256 + paramuc[0], SQLITE_TRANSIENT);
                    }
                    paramuc += 2 + paramuc[1]*256 + paramuc[0];
                    break;
            }
            if (failCode)
                break;
        }
    }
    if (!failCode) {
        //todo: check tautology (duplicate proposition) and contradiction

        if (paramuc != proposition->ch + proposition->size) {
            failCode = 1;
        } else {
            retval = sqlite3_step(insertSt);
            //todo: check return value
        }
    }
    free(bindTypes);
    free(insertStCh);

    //todo: check return value
    sqlite3_finalize(insertSt);
    return failCode;
}

int DeleteProposition(struct UCArr *proposition) {
    //todo: delete the proposition from default table or predicate table
    //todo: and then check the references in raw bytes table, if no other existent references, delete these raw bytes records.
    return EXIT_SUCCESS;
}

int UpdatePropostion(struct UCArr *newp, struct UCArr *oldp) {
    return EXIT_SUCCESS;
}

int SelectPropostion(struct UCArr *proposition) {
    return EXIT_SUCCESS;
}

int CreatePredicateTable(struct UCArr *predicate) {
    if (ParsePredicate(predicate, SNLSpellLookup)) {
        printf("Invalid predicate");
        return EXIT_FAILURE;
    }

    sqlite3_clear_bindings(predicatesSelEqPred);
    sqlite3_reset(predicatesSelEqPred);
    sqlite3_bind_blob(predicatesSelEqPred, 1, predicate->ch, predicate->size, SQLITE_TRANSIENT);
    int retval = sqlite3_step(predicatesSelEqPred);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        return 1;
    } else if (retval == SQLITE_DONE) {
        sqlite3_clear_bindings(virtpredsSelEqPred);
        sqlite3_reset(virtpredsSelEqPred);
        sqlite3_bind_blob(virtpredsSelEqPred, 1, predicate->ch, predicate->size, SQLITE_TRANSIENT);
        retval = sqlite3_step(virtpredsSelEqPred);
        //todo: check return value
        if (retval == SQLITE_ROW) {
            return 1;
        }
    }

    //todo: check tautology (duplicate proposition) and contradiction

    unsigned char* createTabStmt = malloc(16000); //16*998==15968, 16000 should be enough
    strcpy(createTabStmt, "CREATE TABLE ");
    sqlite3_reset(maxLenMaxPredTabName);
    retval = sqlite3_step(maxLenMaxPredTabName);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        size_t numByt = sqlite3_column_bytes(maxLenMaxPredTabName, 0);
        unsigned char ch[numByt + 1];
        ch[numByt] = '0';
        memcpy(ch, sqlite3_column_blob(maxLenMaxPredTabName, 0), numByt);
        SNLColNameIncrement(ch, &numByt);
        if (numByt == 17)
            return 1;
        strncat(createTabStmt, ch, numByt);
    } else if (retval == SQLITE_DONE) {
        strcat(createTabStmt, "a");
    }
    CompleteCreateTabStmt(predicate, createTabStmt);
    char *zErrMsg = NULL;
    sqlite3_exec(databaseA, createTabStmt, NULL, NULL, &zErrMsg);
    //todo: check
    if (zErrMsg != NULL) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }

    sqlite3_clear_bindings(predicatesIns);
    sqlite3_reset(predicatesIns);
    sqlite3_bind_blob(predicatesIns, 1, createTabStmt + sizeof ("CREATE TABLE ") - 1, strcspn(createTabStmt + sizeof ("CREATE TABLE ") - 1, "("), SQLITE_TRANSIENT);
    sqlite3_bind_blob(predicatesIns, 2, predicate->ch, predicate->size, SQLITE_TRANSIENT);
    retval = sqlite3_step(predicatesIns);
    //todo: check

    free(createTabStmt);
    return EXIT_SUCCESS;
}

//todo: add fail code

int CreateVirtualPredicateTable(struct UCArr *predicate) {
    if (ParsePredicate(predicate, SNLSpellLookup)) {
        printf("Invalid predicate");
        return EXIT_FAILURE;
    }

    sqlite3_clear_bindings(predicatesSelEqPred);
    sqlite3_reset(predicatesSelEqPred);
    sqlite3_bind_blob(predicatesSelEqPred, 1, predicate->ch, predicate->size, SQLITE_TRANSIENT);
    int retval = sqlite3_step(predicatesSelEqPred);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        return 1;
    } else if (retval == SQLITE_DONE) {
        sqlite3_clear_bindings(virtpredsSelEqPred);
        sqlite3_reset(virtpredsSelEqPred);
        sqlite3_bind_blob(virtpredsSelEqPred, 1, predicate->ch, predicate->size, SQLITE_TRANSIENT);
        retval = sqlite3_step(virtpredsSelEqPred);
        //todo: check return value
        if (retval == SQLITE_ROW) {
            return 1;
        }
    }

    //todo: check tautology (duplicate proposition) and contradiction

    unsigned char *createVirtTabStmt = malloc(16000); //16*999==15984, 16000 should be enough
    strcpy(createVirtTabStmt, "CREATE TABLE IF NOT EXISTS ");
    CompleteCreateVirtTabStmt(predicate, createVirtTabStmt);
    char *zErrMsg = NULL;
    sqlite3_exec(databaseA, createVirtTabStmt, NULL, NULL, &zErrMsg);
    //todo: check
    if (zErrMsg != NULL) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }

    sqlite3_clear_bindings(virtpredsIns);
    sqlite3_reset(virtpredsIns);
    sqlite3_bind_blob(virtpredsIns, 1, createVirtTabStmt + sizeof ("CREATE TABLE IF NOT EXISTS ") - 1, strcspn(createVirtTabStmt + sizeof ("CREATE TABLE IF NOT EXISTS ") - 1, "("), SQLITE_TRANSIENT);
    sqlite3_bind_blob(virtpredsIns, 2, createVirtTabStmt + sizeof ("CREATE TABLE IF NOT EXISTS ") - 1, strcspn(createVirtTabStmt + sizeof ("CREATE TABLE IF NOT EXISTS ") - 1, "("), SQLITE_TRANSIENT);
    sqlite3_bind_blob(virtpredsIns, 3, predicate->ch, predicate->size, SQLITE_TRANSIENT);
    retval = sqlite3_step(virtpredsIns);
    //todo: check

    free(createVirtTabStmt);
    return EXIT_SUCCESS;
}

int SuggestPredicateTable(struct UCArr *out) {
    //suggestions about create/migrate/DROP

    return EXIT_SUCCESS;
}

int MigratePredicateTable(struct UCArr *tableName, struct UCArr *targetTableName, struct UCArr *migrationMapping) {
    return EXIT_SUCCESS;
}

int DropPredicateTablePreview(struct UCArr *tableName) {
    //todo: list all other table predicates that has nouns related to this one.

    return EXIT_SUCCESS;
}

/*
 * All data in that table will be lost.
 */
int DropPredicateTable(struct UCArr *tableName) {
    //todo: if inssubn contains some id which is contain in this table, and other tables have no proposition containing it, then it fails

    //todo: delete from predicates/virtpreds

    //todo: delete from a virtual / drop the table

    return EXIT_SUCCESS;
}

//todo: this function may need to be renamed, you have better identifier mechanism now (relative clause)

int SetNounIdentifierPattern(struct UCArr *pattern, struct UCArr *applicationScope) {
    //application scope may contain predicate table name, predicate clause in table, proposition.

    //It can partially/completely OVERWRITE previously set noun identifier pattern!!!
    //Set to default by using pattern == NULL

    //the setting is about: insert/update/delete propositions about patterns
    return EXIT_SUCCESS;
}

//todo: this function may need to be renamed, you have better identifier mechanism now (relative clause)

int SuggestNounIdentifierPattern(struct UCArr *outPattern, struct UCArr *outApplicationScope) {
    //suggestions about using better pattern or undo bad pattern settings
    return EXIT_SUCCESS;
}

//todo: change to use enum

int SetPredicateHierarchy(struct UCArr *pattern, unsigned char relationship) {
    //relationship can be i/0:independent, p:superset, b:subset, m:Mutually exclusive

    //It can partially/completely OVERWRITE previously set noun predicate hierarchy!!!
    //Set to default (independent relationship, no restriction) by using pattern == NULL or relationship == i/0

    //the setting is about: insert/update/delete propositions about patterns
    //the setting then finds out all redundant propositions about patterns, and delete them, for saving space, or there will be a huge number of propositions about hierarchy
    return EXIT_SUCCESS;
}

int GuessPredicateHierarchy(struct UCArr *outPattern) {
    //based on statistics of propositions. Guess relationships that haven't been added to database.
    return EXIT_SUCCESS;
}

int SQLiteConfigOpenPragmaBeginTran() {
    //todo: any sqlite3_config ?

    char *zErrMsg = NULL;

    //todo: load the paths from config file.
    sqlite3_open(dbapath, &databaseA);

    sqlite3_exec(databaseA, "PRAGMA locking_mode = EXCLUSIVE", NULL, NULL, &zErrMsg);
    sqlite3_exec(databaseA, "PRAGMA synchronous = OFF", NULL, NULL, &zErrMsg);
    sqlite3_exec(databaseA, "PRAGMA journal_mode = OFF", NULL, NULL, &zErrMsg);
    sqlite3_exec(databaseA, "PRAGMA temp_store = MEMORY", NULL, NULL, &zErrMsg);
    if (sqlite3_exec(databaseA, "BEGIN EXCLUSIVE", NULL, NULL, &zErrMsg))
        printf("databaseA %s", zErrMsg);

    if (zErrMsg != NULL) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }

    int retval;
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO b values(?)", -1, &insertStmtForB, NULL);
    retval = sqlite3_prepare_v2(databaseA, "INSERT INTO a values(?)", -1, &insertDefaultPro, NULL);

    //retval = sqlite3_prepare_v2(databaseA, "select * from sqlite_master WHERE type='table' AND name=?", -1, &sqliteMasterSelEqTabName, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select * from predicates where b=?", -1, &predicatesSelEqPred, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select * from virtpreds where c=?", -1, &virtpredsSelEqPred, NULL);
    //retval = sqlite3_prepare_v2(databaseA, "select max(b) from virtpreds where a=?", -1, &virtpredsSelMaxIdEqTabn, NULL);
    retval = sqlite3_prepare_v2(databaseA, "insert into virtpreds values (?,(select ifnull(max(b),0)+1 from virtpreds where a=?),?)", -1, &virtpredsIns, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select a from synonym where b=?", -1, &synonymSelEqInfl, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select b from subepit where a=?", -1, &subepitSelEqSubep, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select b from epithet where a=?", -1, &epithetSelEqSpell, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select a from dignoun where a=?", -1, &dignounSelEqSpell, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select a from bytnoun where a=?", -1, &bytnounSelEqSpell, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select b from subnoun where a=?", -1, &subnounSelEqSub, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select b,c from abbrnoun where a=?", -1, &abbrnounSelEqAbbr, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select b from absidiss where a=?", -1, &absidissSelEqSpell, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select b from conidiss where a=?", -1, &conidissSelEqSpell, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select a from absroot where a=?", -1, &absrootSelEqSpell, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select a from conroot where a=?", -1, &conrootSelEqSpell, NULL);
    retval = sqlite3_prepare_v2(databaseA, "select a from predicates order by length(a) desc,a desc limit 1", -1, &maxLenMaxPredTabName, NULL);
    retval = sqlite3_prepare_v2(databaseA, "insert into predicates values (?,?)", -1, &predicatesIns, NULL);

    return EXIT_SUCCESS;
}

int SQLiteCommitTranClose() {

    sqlite3_finalize(insertStmtForB);
    sqlite3_finalize(insertDefaultPro);

    //sqlite3_finalize(sqliteMasterSelEqTabName);
    sqlite3_finalize(predicatesSelEqPred);
    sqlite3_finalize(virtpredsSelEqPred);
    //sqlite3_finalize(virtpredsSelMaxIdEqTabn);
    sqlite3_finalize(virtpredsIns);
    sqlite3_finalize(synonymSelEqInfl);
    sqlite3_finalize(subepitSelEqSubep);
    sqlite3_finalize(epithetSelEqSpell);
    sqlite3_finalize(dignounSelEqSpell);
    sqlite3_finalize(bytnounSelEqSpell);
    sqlite3_finalize(subnounSelEqSub);
    sqlite3_finalize(abbrnounSelEqAbbr);
    sqlite3_finalize(absidissSelEqSpell);
    sqlite3_finalize(conidissSelEqSpell);
    sqlite3_finalize(absrootSelEqSpell);
    sqlite3_finalize(conrootSelEqSpell);
    sqlite3_finalize(maxLenMaxPredTabName);
    sqlite3_finalize(predicatesIns);

    char *zErrMsg = NULL;

    if (sqlite3_exec(databaseA, "COMMIT", NULL, NULL, &zErrMsg))
        printf("databaseA %s", zErrMsg);
    //todo: do something if error occurred
    if (zErrMsg != NULL) {
        free(zErrMsg);
    }

    if (sqlite3_close(databaseA) == SQLITE_OK) {
        databaseA = NULL;
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

int InsertRawBytes(unsigned char *ch, size_t size, sqlite3_int64 *rowId) {

    //todo: do somthing if error occurs
    sqlite3_clear_bindings(insertStmtForB);
    sqlite3_reset(insertStmtForB);
    sqlite3_bind_blob(insertStmtForB, 1, ch, size, SQLITE_TRANSIENT);
    int retval = sqlite3_step(insertStmtForB);
    *rowId = sqlite3_last_insert_rowid(databaseA);
    return EXIT_SUCCESS;
}
