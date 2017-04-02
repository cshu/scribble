#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SNLTypes.h"

enum ClauseStatus {
    CLAUSESTATUS_STARTCLAUSE,
    CLAUSESTATUS_BEHINDAT,
    CLAUSESTATUS_BEHINDPOUND,
    CLAUSESTATUS_BEHINDATCOLID,
    CLAUSESTATUS_BEHINDPOUNDCOLID,
    CLAUSESTATUS_BEHINDSINGLESPACE,
    CLAUSESTATUS_BEHINDCOMMA,
    CLAUSESTATUS_ENDNOUNCLAUSE,
    CLAUSESTATUS_ENDRELATIVECLAUSE,
    CLAUSESTATUS_SPELL,
    CLAUSESTATUS_UNKNOWN
};


//todo: SNL Interpret to Commands (consider in the future)

//todo: short code, but performance is not the best

static void SNLColNameDecrement(unsigned char *chkColNameBuf, size_t *gsize) {
    if (*gsize == 1) {
        if (chkColNameBuf[0] == 'a') {
            chkColNameBuf[0] = 'z';
            --*gsize;
        } else
            chkColNameBuf[0]--;
    } else {
        if (chkColNameBuf[*gsize - 1] == '0') {
            --*gsize;
            chkColNameBuf[*gsize] = '9';
            SNLColNameDecrement(chkColNameBuf, gsize);
            ++*gsize;
        } else
            chkColNameBuf[*gsize - 1]--;
    }
}

/*
 * This function assumes that chkColNameBuf has redundant trailing '0' characters for possible promotion.
 */
void SNLColNameIncrement(unsigned char *chkColNameBuf, size_t *gsize) {
    if (*gsize == 1) {
        if (chkColNameBuf[0] == 'z') {
            chkColNameBuf[0] = 'a';
            ++*gsize;
        } else
            chkColNameBuf[0]++;
    } else {
        if (chkColNameBuf[*gsize - 1] == '9') {
            --*gsize;
            chkColNameBuf[*gsize] = '0';
            SNLColNameIncrement(chkColNameBuf, gsize);
            ++*gsize;
        } else
            chkColNameBuf[*gsize - 1]++;
    }
}

static void SetTheGreaterColName(unsigned char **greatestColName, size_t *gsize, unsigned char *challengerColName, size_t csize) {
    if (csize > *gsize) {
        *greatestColName = challengerColName;
        *gsize = csize;
    } else if (csize == *gsize) {
        if (memcmp(*greatestColName, challengerColName, csize) < 0) {
            *greatestColName = challengerColName;
            *gsize = csize;
        }
    }
}

static int ValidateColName(unsigned char **greatestColName, size_t *gsize, unsigned char *challengerColName, size_t csize) {
    if (csize < *gsize) {
        return 0;
    } else if (csize == *gsize) {
        if (memcmp(*greatestColName, challengerColName, csize) < 0) {
            unsigned char uc[csize];
            memcpy(uc, challengerColName, csize);
            SNLColNameDecrement(uc, &csize);
            if (csize == *gsize && memcmp(*greatestColName, uc, csize) == 0) {
                if (csize == 3 && uc[0] == 'h' && uc[1] == '1' && uc[1] == '1')
                    return 1;
                *greatestColName = challengerColName;
                return 0;
            } else
                return 1;
        } else
            return 0;
    } else if (csize = *gsize + 1) {
        if (*gsize == 0) {
            if (challengerColName[0] == 'a') {
                *greatestColName = challengerColName;
                *gsize = csize;
                return 0;
            } else
                return 1;
        } else {
            unsigned char uc[csize];
            memcpy(uc, challengerColName, csize);
            SNLColNameDecrement(uc, &csize);
            if (csize == *gsize && memcmp(*greatestColName, uc, csize) == 0) {
                *greatestColName = challengerColName;
                ++*gsize;
                return 0;
            } else
                return 1;
        }
    } else {
        return 1;
    }
}

enum parsePropositionResult ParseProposition(struct UCArr *proposition, enum snlSpellAndRefLookupResult(*SNLSpellAndRefLookup)(unsigned char *, size_t, enum snlSpellAndRefLookupType)) {

    //todo: add failure reason for the return value

    if (proposition == NULL || proposition->size == 0)
        return PARSEPROPOSITIONRESULT_INVALID;

    enum ClauseStatus status = CLAUSESTATUS_STARTCLAUSE;

    enum ClauseType {
        CLAUSETYPE_INDEPENDENT,
        CLAUSETYPE_NOUN,
        CLAUSETYPE_RELATIVE
    };

    enum SegmentType {
        SEGMENTTYPE_NULL,
        SEGMENTTYPE_NOUN,
        SEGMENTTYPE_EPITHET,
        SEGMENTTYPE_UNKNOWN
    };
    enum SegmentType prevSegmentType = SEGMENTTYPE_NULL; //relative clause has no type, so it's treated as NOUN type, to refer to the noun it describes

    size_t snlSpellOrRefStartInd = 0;

    struct DLLInt *listOfClauseType = InitDLLInt();
    listOfClauseType->value = CLAUSETYPE_INDEPENDENT;

    enum parsePropositionResult retVal = PARSEPROPOSITIONRESULT_VALIDPRED;

    unsigned char *greatestColName;
    size_t greatestColNameSize = 0;

    //todo: besides checking punctuation, also check the grammar of SNL, make sure its grammar is correct
    for (size_t i = 0; i < proposition->size; i++) {
        //todo: ugly code, rewrite this part
        switch (proposition->ch[i]) {
            case '!':
            case '"':
            case '$':
            case '%':
            case '&':
            case '\'':
            case '*':
            case '+':
            case '.':
            case '/':
            case ':':
            case ';':
            case '<':
            case '=':
            case '>':
            case '?':
            case '\\':
            case '^':
            case '`':
                FreeDLLInt(listOfClauseType);
                return PARSEPROPOSITIONRESULT_INVALID;
                break;
            default:
                if (proposition->ch[i] > 'z' || proposition->ch[i] < ' ') {
                    FreeDLLInt(listOfClauseType);
                    return PARSEPROPOSITIONRESULT_INVALID;
                }
                break;
        }

        switch (status) {
            case CLAUSESTATUS_STARTCLAUSE:
                switch (proposition->ch[i]) {
                    case '[':
                        if (listOfClauseType->next == NULL) {
                            listOfClauseType->next = MallocDLLInt();
                            listOfClauseType->next->prev = listOfClauseType;
                            listOfClauseType->next->next = NULL;
                        }
                        listOfClauseType = listOfClauseType->next;
                        listOfClauseType->value = CLAUSETYPE_NOUN;
                        status = CLAUSESTATUS_STARTCLAUSE;
                        break;
                    case '@':
                        status = CLAUSESTATUS_BEHINDAT;
                        break;
                    case '#':
                        status = CLAUSESTATUS_BEHINDPOUND;
                        break;
                    case ' ':
                    case ',':
                    case ']':
                    case '(':
                    case ')':
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    default:
                        snlSpellOrRefStartInd = i;
                        status = CLAUSESTATUS_SPELL;
                        break;
                }
                break;
            case CLAUSESTATUS_BEHINDAT:
                if (proposition->ch[i] > '0' && proposition->ch[i] < ':') {
                    //status = CLAUSESTATUS_BEHINDATROWID;
                    snlSpellOrRefStartInd = i;
                } else if (proposition->ch[i] > '`' && proposition->ch[i] < '{') {
                    status = CLAUSESTATUS_BEHINDATCOLID;
                    snlSpellOrRefStartInd = i;
                } else
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                break;
            case CLAUSESTATUS_BEHINDPOUND:
                if (proposition->ch[i] > '`' && proposition->ch[i] < '{') {
                    status = CLAUSESTATUS_BEHINDPOUNDCOLID;
                    snlSpellOrRefStartInd = i;
                } else
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                break;
                //case CLAUSESTATUS_BEHINDATROWID:
                if (proposition->ch[i] == '@') {
                    if (SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT == SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_REF))
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else
                        status = CLAUSESTATUS_BEHINDAT;
                } else if (proposition->ch[i] == '#') {
                    if (SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT == SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_REF))
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else
                        status = CLAUSESTATUS_BEHINDPOUND;
                } else if (proposition->ch[i] == ' ') {
                    if (SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT == SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_REF))
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        status = CLAUSESTATUS_BEHINDSINGLESPACE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                    }
                } else if (proposition->ch[i] == ',') {
                    if (SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT == SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_REF))
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        status = CLAUSESTATUS_BEHINDCOMMA;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                    }
                } else if (proposition->ch[i] == ']') {
                    if (SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT == SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_REF))
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else if (listOfClauseType->value != CLAUSETYPE_NOUN)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        status = CLAUSESTATUS_ENDNOUNCLAUSE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                        listOfClauseType = listOfClauseType->prev;
                    }
                } else if (proposition->ch[i] == ')') {
                    if (SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT == SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_REF))
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else if (listOfClauseType->value != CLAUSETYPE_RELATIVE)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                        listOfClauseType = listOfClauseType->prev;
                    }
                } else if (proposition->ch[i] < '0' || proposition->ch[i] > '9') {
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                }
                break;
            case CLAUSESTATUS_BEHINDATCOLID:
            case CLAUSESTATUS_BEHINDPOUNDCOLID:
                if (proposition->ch[i] == '@') {
                    SetTheGreaterColName(&greatestColName, &greatestColNameSize, proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                    status = CLAUSESTATUS_BEHINDAT;
                } else if (proposition->ch[i] == '#') {
                    SetTheGreaterColName(&greatestColName, &greatestColNameSize, proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                    status = CLAUSESTATUS_BEHINDPOUND;
                } else if (proposition->ch[i] == ' ') {
                    SetTheGreaterColName(&greatestColName, &greatestColNameSize, proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                    status = CLAUSESTATUS_BEHINDSINGLESPACE;
                    prevSegmentType = SEGMENTTYPE_NOUN;
                } else if (proposition->ch[i] == ',') {
                    if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        SetTheGreaterColName(&greatestColName, &greatestColNameSize, proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                        status = CLAUSESTATUS_BEHINDCOMMA;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                    }
                } else if (proposition->ch[i] == ']') {
                    if (listOfClauseType->value != CLAUSETYPE_NOUN)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        status = CLAUSESTATUS_ENDNOUNCLAUSE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                        listOfClauseType = listOfClauseType->prev;
                        SetTheGreaterColName(&greatestColName, &greatestColNameSize, proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                    }
                } else if (proposition->ch[i] == ')') {
                    if (listOfClauseType->value != CLAUSETYPE_RELATIVE)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                        listOfClauseType = listOfClauseType->prev;
                        SetTheGreaterColName(&greatestColName, &greatestColNameSize, proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                    }
                } else if (proposition->ch[i] < '0' || proposition->ch[i] > '9') {
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                }
                break;
            case CLAUSESTATUS_BEHINDSINGLESPACE:
                switch (proposition->ch[i]) {
                    case ' ':
                    case ',':
                    case ']':
                    case ')':
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    case '@':
                        if (prevSegmentType == SEGMENTTYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else
                            status = CLAUSESTATUS_BEHINDAT;
                        break;
                    case '#':
                        if (prevSegmentType == SEGMENTTYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else
                            status = CLAUSESTATUS_BEHINDPOUND;
                        break;
                    case '[':
                        if (prevSegmentType == SEGMENTTYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            if (listOfClauseType->next == NULL) {
                                listOfClauseType->next = MallocDLLInt();
                                listOfClauseType->next->prev = listOfClauseType;
                                listOfClauseType->next->next = NULL;
                            }
                            listOfClauseType = listOfClauseType->next;
                            listOfClauseType->value = CLAUSETYPE_NOUN;
                            status = CLAUSESTATUS_STARTCLAUSE;
                        }
                        break;
                    case '(':
                        if (prevSegmentType != SEGMENTTYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            if (listOfClauseType->next == NULL) {
                                listOfClauseType->next = MallocDLLInt();
                                listOfClauseType->next->prev = listOfClauseType;
                                listOfClauseType->next->next = NULL;
                            }
                            listOfClauseType = listOfClauseType->next;
                            listOfClauseType->value = CLAUSETYPE_RELATIVE;
                            status = CLAUSESTATUS_STARTCLAUSE;
                        }
                        break;
                    default:
                        snlSpellOrRefStartInd = i;
                        status = CLAUSESTATUS_SPELL;
                        break;
                }
                break;
            case CLAUSESTATUS_BEHINDCOMMA:
                if (proposition->ch[i] == ' ')
                    status = CLAUSESTATUS_STARTCLAUSE;
                else
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                break;
            case CLAUSESTATUS_ENDNOUNCLAUSE:
            case CLAUSESTATUS_ENDRELATIVECLAUSE:
                switch (proposition->ch[i]) {
                    case ' ':
                        status = CLAUSESTATUS_BEHINDSINGLESPACE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                        break;
                    case ',':
                        if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            status = CLAUSESTATUS_BEHINDCOMMA;
                            prevSegmentType = SEGMENTTYPE_NOUN;
                        }
                        break;
                    case ']':
                        if (listOfClauseType->value != CLAUSETYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            status = CLAUSESTATUS_ENDNOUNCLAUSE;
                            prevSegmentType = SEGMENTTYPE_NOUN;
                            listOfClauseType = listOfClauseType->prev;
                        }
                        break;
                    case ')':
                        if (listOfClauseType->value != CLAUSETYPE_RELATIVE)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                            prevSegmentType = SEGMENTTYPE_NOUN;
                            listOfClauseType = listOfClauseType->prev;
                        }
                        break;
                    default:
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                }
                break;
            case CLAUSESTATUS_SPELL:
                switch (proposition->ch[i]) {
                    case '@':
                    case '#':
                    case '[':
                    case '(':
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    case ' ':
                        switch (SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_SPELL)) {
                            case SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT:
                            default:
                                retVal = PARSEPROPOSITIONRESULT_INVALID;
                                break;
                            case SNLSPELLANDREFLOOKUPRESULT_NOUN:
                                status = CLAUSESTATUS_BEHINDSINGLESPACE;
                                prevSegmentType = SEGMENTTYPE_NOUN;
                                break;
                            case SNLSPELLANDREFLOOKUPRESULT_EPITHET:
                                status = CLAUSESTATUS_BEHINDSINGLESPACE;
                                prevSegmentType = SEGMENTTYPE_EPITHET;
                                break;
                        }
                        break;
                    case ',':
                        if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            switch (SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_SPELL)) {
                                case SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT:
                                default:
                                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                                    break;
                                case SNLSPELLANDREFLOOKUPRESULT_NOUN:
                                    status = CLAUSESTATUS_BEHINDCOMMA;
                                    prevSegmentType = SEGMENTTYPE_NOUN;
                                    break;
                                case SNLSPELLANDREFLOOKUPRESULT_EPITHET:
                                    status = CLAUSESTATUS_BEHINDCOMMA;
                                    prevSegmentType = SEGMENTTYPE_EPITHET;
                                    break;
                            }
                        }
                        break;
                    case ']':
                        if (listOfClauseType->value != CLAUSETYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            switch (SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_SPELL)) {
                                case SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT:
                                default:
                                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                                    break;
                                case SNLSPELLANDREFLOOKUPRESULT_NOUN:
                                    status = CLAUSESTATUS_ENDNOUNCLAUSE;
                                    prevSegmentType = SEGMENTTYPE_NOUN;
                                    listOfClauseType = listOfClauseType->prev;
                                    break;
                                case SNLSPELLANDREFLOOKUPRESULT_EPITHET:
                                    status = CLAUSESTATUS_ENDNOUNCLAUSE;
                                    prevSegmentType = SEGMENTTYPE_EPITHET;
                                    listOfClauseType = listOfClauseType->prev;
                                    break;
                            }
                        }
                        break;
                    case ')':
                        if (listOfClauseType->value != CLAUSETYPE_RELATIVE)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            switch (SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_SPELL)) {
                                case SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT:
                                default:
                                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                                    break;
                                case SNLSPELLANDREFLOOKUPRESULT_NOUN:
                                    status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                                    prevSegmentType = SEGMENTTYPE_NOUN;
                                    listOfClauseType = listOfClauseType->prev;
                                    break;
                                case SNLSPELLANDREFLOOKUPRESULT_EPITHET:
                                    status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                                    prevSegmentType = SEGMENTTYPE_EPITHET;
                                    listOfClauseType = listOfClauseType->prev;
                                    break;
                            }
                        }
                        break;
                }
                break;
        }

        if (retVal == PARSEPROPOSITIONRESULT_INVALID) {
            FreeDLLInt(listOfClauseType);
            return PARSEPROPOSITIONRESULT_INVALID;
        }
    }
    if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT) {
        FreeDLLInt(listOfClauseType);
        return PARSEPROPOSITIONRESULT_INVALID;
    }
    FreeDLLInt(listOfClauseType);
    switch (status) {
            //case CLAUSESTATUS_BEHINDATROWID:
            if (SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT == SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, proposition->size - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_REF))
                return PARSEPROPOSITIONRESULT_INVALID;
            break;
        case CLAUSESTATUS_BEHINDATCOLID:
        case CLAUSESTATUS_BEHINDPOUNDCOLID:
            SetTheGreaterColName(&greatestColName, &greatestColNameSize, proposition->ch + snlSpellOrRefStartInd, proposition->size - snlSpellOrRefStartInd);
            break;
        case CLAUSESTATUS_ENDNOUNCLAUSE:
        case CLAUSESTATUS_ENDRELATIVECLAUSE:
            break;
        case CLAUSESTATUS_SPELL:
            switch (SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, proposition->size - snlSpellOrRefStartInd, SNLSPELLANDREFLOOKUPTYPE_SPELL)) {
                case SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT:
                default:
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                    break;
                case SNLSPELLANDREFLOOKUPRESULT_NOUN:
                case SNLSPELLANDREFLOOKUPRESULT_EPITHET:
                    break;
            }
            break;
        default:
            return PARSEPROPOSITIONRESULT_INVALID;
            break;
    }



    //if any column reference like @a is found, check if the naming is sequential.
    if (greatestColNameSize) {
        unsigned char chkColNameBuf[greatestColNameSize];
        memcpy(chkColNameBuf, greatestColName, greatestColNameSize);
        int isColNameAbsent;
        for (;;) {
            SNLColNameDecrement(chkColNameBuf, &greatestColNameSize);
            if (greatestColNameSize == 0)
                break;
            isColNameAbsent = 1;
            for (size_t i = 0; i < proposition->size - greatestColNameSize; i++) {
                if (proposition->ch[i] == '#' || proposition->ch[i] == '@') {
                    if (!memcmp(chkColNameBuf, proposition->ch + i + 1, greatestColNameSize)) {
                        if (i + 1 + greatestColNameSize == proposition->size || proposition->ch[i + 1 + greatestColNameSize] < '0' || proposition->ch[i + 1 + greatestColNameSize] > '9') {
                            isColNameAbsent = 0;
                            break;
                        }
                    }
                }
            }
            if (isColNameAbsent)
                return PARSEPROPOSITIONRESULT_INVALID;
        }
        return PARSEPROPOSITIONRESULT_VALIDPRED;
    } else
        return PARSEPROPOSITIONRESULT_VALIDPROP;
}

int ParsePredicate(struct UCArr *predicate, enum snlSpellLookupResult(*SNLSpellLookup)(const unsigned char *, size_t)) {

    //todo: add failure reason for the return value

    if (predicate == NULL || predicate->size == 0)
        return PARSEPROPOSITIONRESULT_INVALID;

    enum ClauseStatus status = CLAUSESTATUS_STARTCLAUSE;

    enum ClauseType {
        CLAUSETYPE_INDEPENDENT,
        CLAUSETYPE_NOUN,
        CLAUSETYPE_RELATIVE
    };

    enum SegmentType {
        SEGMENTTYPE_NULL,
        SEGMENTTYPE_NOUN,
        SEGMENTTYPE_EPITHET,
        SEGMENTTYPE_UNKNOWN
    };
    enum SegmentType prevSegmentType = SEGMENTTYPE_NULL; //relative clause has no type, so it's treated as NOUN type, to refer to the noun it describes
    enum snlSpellLookupResult lastSpellLookup = snlSpellLookupResult_NONEXISTENT;
    int isIdOrBytRelativeClausePossible = 0;
    int isDigBytRelaClauPotentialUnderConstruct = 0;

    size_t snlSpellOrRefStartInd = 0;

    struct DLLInt *listOfClauseType = InitDLLInt();
    listOfClauseType->value = CLAUSETYPE_INDEPENDENT;

    enum parsePropositionResult retVal = PARSEPROPOSITIONRESULT_VALIDPRED;

    unsigned char *greatestColName;
    size_t greatestColNameSize = 0;

    //todo: besides checking punctuation, also check the grammar of SNL, make sure its grammar is correct
    for (size_t i = 0; i < predicate->size; i++) {
        //todo: ugly code, rewrite this part
        switch (predicate->ch[i]) {
            case '(':
                isDigBytRelaClauPotentialUnderConstruct = 1;
                break;
            case '!':
            case '"':
            case '$':
            case '%':
            case '&':
            case '\'':
            case '*':
            case '+':
            case '.':
            case '/':
            case ':':
            case ';':
            case '<':
            case '=':
            case '>':
            case '?':
            case '\\':
            case '^':
            case '`':
                FreeDLLInt(listOfClauseType);
                return PARSEPROPOSITIONRESULT_INVALID;
                break;
            default:
                if (predicate->ch[i] > 'z' || predicate->ch[i] < ' ') {
                    FreeDLLInt(listOfClauseType);
                    return PARSEPROPOSITIONRESULT_INVALID;
                }
                break;
        }

        switch (status) {
            case CLAUSESTATUS_STARTCLAUSE:
                switch (predicate->ch[i]) {
                    case '[':
                        if (listOfClauseType->next == NULL) {
                            listOfClauseType->next = MallocDLLInt();
                            listOfClauseType->next->prev = listOfClauseType;
                            listOfClauseType->next->next = NULL;
                        }
                        listOfClauseType = listOfClauseType->next;
                        listOfClauseType->value = CLAUSETYPE_NOUN;
                        status = CLAUSESTATUS_STARTCLAUSE;
                        isIdOrBytRelativeClausePossible = 0;
                        isDigBytRelaClauPotentialUnderConstruct = 0;
                        break;
                    case '@':
                        status = CLAUSESTATUS_BEHINDAT;
                        if (isIdOrBytRelativeClausePossible == 1)
                            isIdOrBytRelativeClausePossible = 0;
                        break;
                    case '#':
                        status = CLAUSESTATUS_BEHINDPOUND;
                        if (isIdOrBytRelativeClausePossible == 2)
                            isIdOrBytRelativeClausePossible = 0;
                        break;
                    case ' ':
                    case ',':
                    case ']':
                    case '(':
                    case ')':
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    default:
                        snlSpellOrRefStartInd = i;
                        status = CLAUSESTATUS_SPELL;
                        break;
                }
                break;
            case CLAUSESTATUS_BEHINDAT:
                if (predicate->ch[i] > '`' && predicate->ch[i] < '{') {
                    status = CLAUSESTATUS_BEHINDATCOLID;
                    snlSpellOrRefStartInd = i;
                } else
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                break;
            case CLAUSESTATUS_BEHINDPOUND:
                if (predicate->ch[i] > '`' && predicate->ch[i] < '{') {
                    status = CLAUSESTATUS_BEHINDPOUNDCOLID;
                    snlSpellOrRefStartInd = i;
                } else
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                break;
            case CLAUSESTATUS_BEHINDATCOLID:
            case CLAUSESTATUS_BEHINDPOUNDCOLID:
                if (predicate->ch[i] == '@') {
                    if (ValidateColName(&greatestColName, &greatestColNameSize, predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd)) {
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    }
                    status = CLAUSESTATUS_BEHINDAT;
                    isIdOrBytRelativeClausePossible = 0;
                    isDigBytRelaClauPotentialUnderConstruct = 0;
                } else if (predicate->ch[i] == '#') {
                    if (ValidateColName(&greatestColName, &greatestColNameSize, predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd)) {
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    }
                    status = CLAUSESTATUS_BEHINDPOUND;
                    isIdOrBytRelativeClausePossible = 0;
                    isDigBytRelaClauPotentialUnderConstruct = 0;
                } else if (predicate->ch[i] == ' ') {
                    if (ValidateColName(&greatestColName, &greatestColNameSize, predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd)) {
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    }
                    status = CLAUSESTATUS_BEHINDSINGLESPACE;
                    prevSegmentType = SEGMENTTYPE_NOUN;
                    isIdOrBytRelativeClausePossible = 0;
                    isDigBytRelaClauPotentialUnderConstruct = 0;
                } else if (predicate->ch[i] == ',') {
                    if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        if (ValidateColName(&greatestColName, &greatestColNameSize, predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd)) {
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                            break;
                        }
                        status = CLAUSESTATUS_BEHINDCOMMA;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                    }
                } else if (predicate->ch[i] == ']') {
                    if (listOfClauseType->value != CLAUSETYPE_NOUN)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        status = CLAUSESTATUS_ENDNOUNCLAUSE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                        listOfClauseType = listOfClauseType->prev;
                        if (ValidateColName(&greatestColName, &greatestColNameSize, predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd)) {
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                            break;
                        }
                    }
                } else if (predicate->ch[i] == ')') {
                    if (listOfClauseType->value != CLAUSETYPE_RELATIVE)
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                    else {
                        status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                        listOfClauseType = listOfClauseType->prev;
                        if (ValidateColName(&greatestColName, &greatestColNameSize, predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd)) {
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                            break;
                        }
                        if (!isIdOrBytRelativeClausePossible && isDigBytRelaClauPotentialUnderConstruct) {
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                            break;
                        }
                    }
                } else if (predicate->ch[i] < '0' || predicate->ch[i] > '9') {
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                }
                break;
            case CLAUSESTATUS_BEHINDSINGLESPACE:
                switch (predicate->ch[i]) {
                    case ' ':
                    case ',':
                    case ']':
                    case ')':
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    case '@':
                        if (prevSegmentType == SEGMENTTYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else
                            status = CLAUSESTATUS_BEHINDAT;
                        break;
                    case '#':
                        if (prevSegmentType == SEGMENTTYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else
                            status = CLAUSESTATUS_BEHINDPOUND;
                        break;
                    case '[':
                        if (prevSegmentType == SEGMENTTYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            if (listOfClauseType->next == NULL) {
                                listOfClauseType->next = MallocDLLInt();
                                listOfClauseType->next->prev = listOfClauseType;
                                listOfClauseType->next->next = NULL;
                            }
                            listOfClauseType = listOfClauseType->next;
                            listOfClauseType->value = CLAUSETYPE_NOUN;
                            status = CLAUSESTATUS_STARTCLAUSE;
                        }
                        break;
                    case '(':
                        if (prevSegmentType != SEGMENTTYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            if (listOfClauseType->next == NULL) {
                                listOfClauseType->next = MallocDLLInt();
                                listOfClauseType->next->prev = listOfClauseType;
                                listOfClauseType->next->next = NULL;
                            }
                            listOfClauseType = listOfClauseType->next;
                            listOfClauseType->value = CLAUSETYPE_RELATIVE;
                            status = CLAUSESTATUS_STARTCLAUSE;
                        }
                        break;
                    default:
                        isIdOrBytRelativeClausePossible = 0;
                        snlSpellOrRefStartInd = i;
                        status = CLAUSESTATUS_SPELL;
                        break;
                }
                break;
            case CLAUSESTATUS_BEHINDCOMMA:
                if (predicate->ch[i] == ' ')
                    status = CLAUSESTATUS_STARTCLAUSE;
                else
                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                break;
            case CLAUSESTATUS_ENDNOUNCLAUSE:
            case CLAUSESTATUS_ENDRELATIVECLAUSE:
                switch (predicate->ch[i]) {
                    case ' ':
                        status = CLAUSESTATUS_BEHINDSINGLESPACE;
                        prevSegmentType = SEGMENTTYPE_NOUN;
                        break;
                    case ',':
                        if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            status = CLAUSESTATUS_BEHINDCOMMA;
                            prevSegmentType = SEGMENTTYPE_NOUN;
                            isIdOrBytRelativeClausePossible = 0;
                        }
                        break;
                    case ']':
                        if (listOfClauseType->value != CLAUSETYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            status = CLAUSESTATUS_ENDNOUNCLAUSE;
                            prevSegmentType = SEGMENTTYPE_NOUN;
                            listOfClauseType = listOfClauseType->prev;
                            isIdOrBytRelativeClausePossible = 0;
                        }
                        break;
                    case ')':
                        if (listOfClauseType->value != CLAUSETYPE_RELATIVE)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                            prevSegmentType = SEGMENTTYPE_NOUN;
                            listOfClauseType = listOfClauseType->prev;
                            isIdOrBytRelativeClausePossible = 0;
                        }
                        break;
                    default:
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                }
                break;
            case CLAUSESTATUS_SPELL:
                switch (predicate->ch[i]) {
                    case '@':
                    case '#':
                    case '[':
                    case '(':
                        retVal = PARSEPROPOSITIONRESULT_INVALID;
                        break;
                    case ' ':
                        isDigBytRelaClauPotentialUnderConstruct = 0;
                        lastSpellLookup = SNLSpellLookup(predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                        switch (lastSpellLookup) {
                            case snlSpellLookupResult_NONEXISTENT:
                                retVal = PARSEPROPOSITIONRESULT_INVALID;
                                break;
                            case snlSpellLookupResult_EPITHET:
                            case snlSpellLookupResult_SUBEPIT:
                                status = CLAUSESTATUS_BEHINDSINGLESPACE;
                                prevSegmentType = SEGMENTTYPE_EPITHET;
                                isIdOrBytRelativeClausePossible = 0;
                                break;
                            case snlSpellLookupResult_DIGITS:
                                status = CLAUSESTATUS_BEHINDSINGLESPACE;
                                prevSegmentType = SEGMENTTYPE_NOUN;
                                isIdOrBytRelativeClausePossible = 0;
                                break;
                            case snlSpellLookupResult_BYTNOUN:
                                status = CLAUSESTATUS_BEHINDSINGLESPACE;
                                prevSegmentType = SEGMENTTYPE_NOUN;
                                isIdOrBytRelativeClausePossible = 2;
                                break;
                            default:
                                status = CLAUSESTATUS_BEHINDSINGLESPACE;
                                prevSegmentType = SEGMENTTYPE_NOUN;
                                isIdOrBytRelativeClausePossible = 1;
                                break;
                        }
                        break;
                    case ',':
                        if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            lastSpellLookup = SNLSpellLookup(predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                            switch (lastSpellLookup) {
                                case snlSpellLookupResult_NONEXISTENT:
                                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                                    break;
                                case snlSpellLookupResult_EPITHET:
                                case snlSpellLookupResult_SUBEPIT:
                                    status = CLAUSESTATUS_BEHINDCOMMA;
                                    prevSegmentType = SEGMENTTYPE_EPITHET;
                                    break;
                                default:
                                    status = CLAUSESTATUS_BEHINDCOMMA;
                                    prevSegmentType = SEGMENTTYPE_NOUN;
                                    break;
                            }
                        }
                        break;
                    case ']':
                        if (listOfClauseType->value != CLAUSETYPE_NOUN)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            lastSpellLookup = SNLSpellLookup(predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                            switch (lastSpellLookup) {
                                case snlSpellLookupResult_NONEXISTENT:
                                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                                    break;
                                case snlSpellLookupResult_EPITHET:
                                case snlSpellLookupResult_SUBEPIT:
                                    status = CLAUSESTATUS_ENDNOUNCLAUSE;
                                    prevSegmentType = SEGMENTTYPE_EPITHET;
                                    listOfClauseType = listOfClauseType->prev;
                                    break;
                                default:
                                    status = CLAUSESTATUS_ENDNOUNCLAUSE;
                                    prevSegmentType = SEGMENTTYPE_NOUN;
                                    listOfClauseType = listOfClauseType->prev;
                                    break;
                            }
                        }
                        break;
                    case ')':
                        if (listOfClauseType->value != CLAUSETYPE_RELATIVE)
                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                        else {
                            lastSpellLookup = SNLSpellLookup(predicate->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd);
                            switch (lastSpellLookup) {
                                case snlSpellLookupResult_NONEXISTENT:
                                    retVal = PARSEPROPOSITIONRESULT_INVALID;
                                    break;
                                case snlSpellLookupResult_EPITHET:
                                case snlSpellLookupResult_SUBEPIT:
                                    status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                                    prevSegmentType = SEGMENTTYPE_EPITHET;
                                    listOfClauseType = listOfClauseType->prev;
                                    isIdOrBytRelativeClausePossible = 0;
                                    isDigBytRelaClauPotentialUnderConstruct = 0;
                                    break;
                                case snlSpellLookupResult_DIGITS:
                                    status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                                    prevSegmentType = SEGMENTTYPE_NOUN;
                                    listOfClauseType = listOfClauseType->prev;
                                    switch (isIdOrBytRelativeClausePossible) {
                                        case 0:
                                            if (isDigBytRelaClauPotentialUnderConstruct)
                                                retVal = PARSEPROPOSITIONRESULT_INVALID;
                                            break;
                                        case 1:
                                            //todo: check id exists in at least one proposition
                                            break;
                                        case 2:
                                            retVal = PARSEPROPOSITIONRESULT_INVALID;
                                            break;
                                    }
                                    break;
                                default:
                                    status = CLAUSESTATUS_ENDRELATIVECLAUSE;
                                    prevSegmentType = SEGMENTTYPE_NOUN;
                                    listOfClauseType = listOfClauseType->prev;
                                    isIdOrBytRelativeClausePossible = 0;
                                    isDigBytRelaClauPotentialUnderConstruct = 0;
                                    break;
                            }
                        }
                        break;
                }
                break;
        }

        if (retVal == PARSEPROPOSITIONRESULT_INVALID) {
            FreeDLLInt(listOfClauseType);
            return PARSEPROPOSITIONRESULT_INVALID;
        }
    }
    if (listOfClauseType->value != CLAUSETYPE_INDEPENDENT) {
        FreeDLLInt(listOfClauseType);
        return PARSEPROPOSITIONRESULT_INVALID;
    }
    FreeDLLInt(listOfClauseType);
    switch (status) {
        case CLAUSESTATUS_BEHINDATCOLID:
        case CLAUSESTATUS_BEHINDPOUNDCOLID:
            if (ValidateColName(&greatestColName, &greatestColNameSize, predicate->ch + snlSpellOrRefStartInd, predicate->size - snlSpellOrRefStartInd)) {
                return PARSEPROPOSITIONRESULT_INVALID;
            }
            break;
        case CLAUSESTATUS_ENDNOUNCLAUSE:
        case CLAUSESTATUS_ENDRELATIVECLAUSE:
            break;
        case CLAUSESTATUS_SPELL:
            lastSpellLookup = SNLSpellLookup(predicate->ch + snlSpellOrRefStartInd, predicate->size - snlSpellOrRefStartInd);
            if (lastSpellLookup == snlSpellLookupResult_NONEXISTENT) {
                return PARSEPROPOSITIONRESULT_INVALID;
            }
            break;
        default:
            return PARSEPROPOSITIONRESULT_INVALID;
            break;
    }

    return 0;
}

/**
 * //todo: this is actually a IIDAL function
 * @param predicate
 * @param createVirtTabStmt
 */
void CompleteCreateVirtTabStmt(struct UCArr *predicate, unsigned char *createVirtTabStmt) {
    unsigned char inteP[4] = {'0'}; //4 is enough because Number of params cannot exceed 998
    unsigned char inteN[4] = {'0'};
    unsigned char blobP[4] = {'0'};
    unsigned char blobN[4] = {'0'};
    enum ClauseStatus status = CLAUSESTATUS_STARTCLAUSE;
    unsigned char nextParaName[] = {'a', '0', '0'};
    size_t nPNLen = 1;
    unsigned char ifSpace[] = "if ";
    unsigned char isNotNv[] = " is_not Null_value";
    unsigned char isNv[] = "is Null_value";
    for (size_t i = 0; i < predicate->size; i++) {
        switch (status) {
            case CLAUSESTATUS_BEHINDAT:
                if (i + nPNLen <= predicate->size && memcmp(nextParaName, predicate->ch + i, nPNLen) == 0) {
                    if (i >= sizeof (ifSpace) &&
                            memcmp(ifSpace, predicate->ch + i - sizeof (ifSpace), sizeof (ifSpace) - 1) == 0 &&
                            (i == sizeof (ifSpace) || predicate->ch[i - sizeof (ifSpace) - 1] == ' ' || predicate->ch[i - sizeof (ifSpace) - 1] == '[' || predicate->ch[i - sizeof (ifSpace) - 1] == '(') &&
                            (
                            i + nPNLen + sizeof (isNotNv) - 1 <= predicate->size &&
                            memcmp(isNotNv, predicate->ch + i + nPNLen, sizeof (isNotNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNotNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '[')
                            ||
                            i + nPNLen + sizeof (isNv) - 1 <= predicate->size &&
                            memcmp(isNv, predicate->ch + i + nPNLen, sizeof (isNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '[')
                            )
                            )
                        incrementLiteralUnsignedInt(blobN);
                    else
                        incrementLiteralUnsignedInt(blobP);
                    SNLColNameIncrement(nextParaName, &nPNLen);
                }
                status = CLAUSESTATUS_UNKNOWN;
                break;
            case CLAUSESTATUS_BEHINDPOUND:
                if (i + nPNLen <= predicate->size && memcmp(nextParaName, predicate->ch + i, nPNLen) == 0) {
                    if (i >= sizeof (ifSpace) &&
                            memcmp(ifSpace, predicate->ch + i - sizeof (ifSpace), sizeof (ifSpace) - 1) == 0 &&
                            (i == sizeof (ifSpace) || predicate->ch[i - sizeof (ifSpace) - 1] == ' ' || predicate->ch[i - sizeof (ifSpace) - 1] == '[' || predicate->ch[i - sizeof (ifSpace) - 1] == '(') &&
                            (
                            i + nPNLen + sizeof (isNotNv) - 1 <= predicate->size &&
                            memcmp(isNotNv, predicate->ch + i + nPNLen, sizeof (isNotNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNotNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '[')
                            ||
                            i + nPNLen + sizeof (isNv) - 1 <= predicate->size &&
                            memcmp(isNv, predicate->ch + i + nPNLen, sizeof (isNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '[')
                            )
                            )
                        incrementLiteralUnsignedInt(inteN);
                    else
                        incrementLiteralUnsignedInt(inteP);
                    SNLColNameIncrement(nextParaName, &nPNLen);
                }
                status = CLAUSESTATUS_UNKNOWN;
                break;
            default:
                switch (predicate->ch[i]) {
                    case '@':
                        status = CLAUSESTATUS_BEHINDAT;
                        break;
                    case '#':
                        status = CLAUSESTATUS_BEHINDPOUND;
                        break;
                }
                break;
        }
    }

    strcat(createVirtTabStmt, "i");
    strcat(createVirtTabStmt, inteP);
    strcat(createVirtTabStmt, "b");
    strcat(createVirtTabStmt, blobP);
    strcat(createVirtTabStmt, "n");
    strcat(createVirtTabStmt, inteN);
    strcat(createVirtTabStmt, "n");
    strcat(createVirtTabStmt, blobN);
    strcat(createVirtTabStmt, "(vtn INTEGER,");
    nextParaName[0] = 'a';
    nextParaName[1] = '0';
    nextParaName[2] = '0';
    nPNLen = 1;
    unsigned char counter[4] = {'0'};
    for (;;) {
        if (strcmp(counter, inteP) == 0)
            break;
        incrementLiteralUnsignedInt(counter);
        strncat(createVirtTabStmt, nextParaName, nPNLen);
        strcat(createVirtTabStmt, " INTEGER,");
        SNLColNameIncrement(nextParaName, &nPNLen);
    }
    counter[0] = '0';
    counter[1] = 0;
    counter[2] = 0;
    counter[3] = 0;
    for (;;) {
        if (strcmp(counter, blobP) == 0)
            break;
        incrementLiteralUnsignedInt(counter);
        strncat(createVirtTabStmt, nextParaName, nPNLen);
        strcat(createVirtTabStmt, " BLOB,");
        SNLColNameIncrement(nextParaName, &nPNLen);
    }
    counter[0] = '0';
    counter[1] = 0;
    counter[2] = 0;
    counter[3] = 0;
    for (;;) {
        if (strcmp(counter, inteN) == 0)
            break;
        incrementLiteralUnsignedInt(counter);
        strncat(createVirtTabStmt, nextParaName, nPNLen);
        strcat(createVirtTabStmt, " INTEGER,");
        SNLColNameIncrement(nextParaName, &nPNLen);
    }
    counter[0] = '0';
    counter[1] = 0;
    counter[2] = 0;
    counter[3] = 0;
    for (;;) {
        if (strcmp(counter, blobN) == 0)
            break;
        incrementLiteralUnsignedInt(counter);
        strncat(createVirtTabStmt, nextParaName, nPNLen);
        strcat(createVirtTabStmt, " BLOB,");
        SNLColNameIncrement(nextParaName, &nPNLen);
    }

    strcat(createVirtTabStmt, "PRIMARY KEY(vtn,");
    nextParaName[0] = 'a';
    nextParaName[1] = '0';
    nextParaName[2] = '0';
    nPNLen = 1;
    counter[0] = '0';
    counter[1] = 0;
    counter[2] = 0;
    counter[3] = 0;
    for (;;) {
        if (strcmp(counter, inteP) == 0)
            break;
        incrementLiteralUnsignedInt(counter);
        strncat(createVirtTabStmt, nextParaName, nPNLen);
        strcat(createVirtTabStmt, ",");
        SNLColNameIncrement(nextParaName, &nPNLen);
    }
    counter[0] = '0';
    counter[1] = 0;
    counter[2] = 0;
    counter[3] = 0;
    for (;;) {
        if (strcmp(counter, blobP) == 0)
            break;
        incrementLiteralUnsignedInt(counter);
        strncat(createVirtTabStmt, nextParaName, nPNLen);
        strcat(createVirtTabStmt, ",");
        SNLColNameIncrement(nextParaName, &nPNLen);
    }
    createVirtTabStmt[strlen(createVirtTabStmt) - 1] = ')';
    strcat(createVirtTabStmt, ")WITHOUT ROWID");
}

/**
 * //todo: this is actually a IIDAL function
 * @param predicate
 * @param createTabStmt
 */
void CompleteCreateTabStmt(struct UCArr *predicate, unsigned char *createTabStmt) {
    strcat(createTabStmt, "(");
    enum ClauseStatus status = CLAUSESTATUS_STARTCLAUSE;
    unsigned char nextParaName[3] = {'a', '0', '0'};
    size_t nPNLen = 1;
    unsigned char ifSpace[] = "if ";
    unsigned char isNotNv[] = " is_not Null_value";
    unsigned char isNv[] = "is Null_value";
    unsigned char *pkPart = malloc(4000); //4*998==3992, 4000 should be enough
    strcpy(pkPart, "PRIMARY KEY(");
    for (size_t i = 0; i < predicate->size; i++) {
        switch (status) {
            case CLAUSESTATUS_BEHINDAT:
                if (i + nPNLen <= predicate->size && memcmp(nextParaName, predicate->ch + i, nPNLen) == 0) {
                    if (i >= sizeof (ifSpace) &&
                            memcmp(ifSpace, predicate->ch + i - sizeof (ifSpace), sizeof (ifSpace) - 1) == 0 &&
                            (i == sizeof (ifSpace) || predicate->ch[i - sizeof (ifSpace) - 1] == ' ' || predicate->ch[i - sizeof (ifSpace) - 1] == '[' || predicate->ch[i - sizeof (ifSpace) - 1] == '(') &&
                            (
                            i + nPNLen + sizeof (isNotNv) - 1 <= predicate->size &&
                            memcmp(isNotNv, predicate->ch + i + nPNLen, sizeof (isNotNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNotNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '[')
                            ||
                            i + nPNLen + sizeof (isNv) - 1 <= predicate->size &&
                            memcmp(isNv, predicate->ch + i + nPNLen, sizeof (isNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '[')
                            )
                            ) {
                    } else {
                        strncat(pkPart, nextParaName, nPNLen);
                        strcat(pkPart, ",");
                    }
                    strncat(createTabStmt, nextParaName, nPNLen);
                    strcat(createTabStmt, " BLOB,");
                    SNLColNameIncrement(nextParaName, &nPNLen);
                }
                status = CLAUSESTATUS_UNKNOWN;
                break;
            case CLAUSESTATUS_BEHINDPOUND:
                if (i + nPNLen <= predicate->size && memcmp(nextParaName, predicate->ch + i, nPNLen) == 0) {
                    if (i >= sizeof (ifSpace) &&
                            memcmp(ifSpace, predicate->ch + i - sizeof (ifSpace), sizeof (ifSpace) - 1) == 0 &&
                            (i == sizeof (ifSpace) || predicate->ch[i - sizeof (ifSpace) - 1] == ' ' || predicate->ch[i - sizeof (ifSpace) - 1] == '[' || predicate->ch[i - sizeof (ifSpace) - 1] == '(') &&
                            (
                            i + nPNLen + sizeof (isNotNv) - 1 <= predicate->size &&
                            memcmp(isNotNv, predicate->ch + i + nPNLen, sizeof (isNotNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNotNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '[')
                            ||
                            i + nPNLen + sizeof (isNv) - 1 <= predicate->size &&
                            memcmp(isNv, predicate->ch + i + nPNLen, sizeof (isNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '[')
                            )
                            ) {
                    } else {
                        strncat(pkPart, nextParaName, nPNLen);
                        strcat(pkPart, ",");
                    }
                    strncat(createTabStmt, nextParaName, nPNLen);
                    strcat(createTabStmt, " INTEGER,");
                    SNLColNameIncrement(nextParaName, &nPNLen);
                }
                status = CLAUSESTATUS_UNKNOWN;
                break;
            default:
                switch (predicate->ch[i]) {
                    case '@':
                        status = CLAUSESTATUS_BEHINDAT;
                        break;
                    case '#':
                        status = CLAUSESTATUS_BEHINDPOUND;
                        break;
                }
                break;
        }
    }
    pkPart[strlen(pkPart) - 1] = ')';
    strcat(createTabStmt, pkPart);
    free(pkPart);
    strcat(createTabStmt, ")WITHOUT ROWID");
}

void GetBindIndicesOfVirtPred(struct UCArr *predicate, unsigned short *bindIndices, unsigned char *bindTypes) {
    unsigned short *bindIndex = bindIndices;
    unsigned char *bindType = bindTypes;
    unsigned short inteP = 1;
    unsigned short inteN = 0;
    unsigned short blobP = 0;
    unsigned short blobN = 0;
    enum ClauseStatus status = CLAUSESTATUS_STARTCLAUSE;
    unsigned char nextParaName[3] = {'a', '0', '0'};
    size_t nPNLen = 1;
    unsigned char ifSpace[] = "if ";
    unsigned char isNotNv[] = " is_not Null_value";
    unsigned char isNv[] = "is Null_value";
    for (size_t i = 0; i < predicate->size; i++) {
        switch (status) {
            case CLAUSESTATUS_BEHINDAT:
                if (i + nPNLen <= predicate->size && memcmp(nextParaName, predicate->ch + i, nPNLen) == 0) {
                    if (i >= sizeof (ifSpace) &&
                            memcmp(ifSpace, predicate->ch + i - sizeof (ifSpace), sizeof (ifSpace) - 1) == 0 &&
                            (i == sizeof (ifSpace) || predicate->ch[i - sizeof (ifSpace) - 1] == ' ' || predicate->ch[i - sizeof (ifSpace) - 1] == '[' || predicate->ch[i - sizeof (ifSpace) - 1] == '(') &&
                            (
                            i + nPNLen + sizeof (isNotNv) - 1 <= predicate->size &&
                            memcmp(isNotNv, predicate->ch + i + nPNLen, sizeof (isNotNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNotNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '[')
                            ||
                            i + nPNLen + sizeof (isNv) - 1 <= predicate->size &&
                            memcmp(isNv, predicate->ch + i + nPNLen, sizeof (isNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '[')
                            )
                            ) {
                        blobN++;
                        bindIndex[0] = blobN;
                        bindType[0] = 3;
                    } else {
                        blobP++;
                        bindIndex[0] = blobP;
                        bindType[0] = 2;
                    }
                    bindIndex++;
                    bindType++;
                    SNLColNameIncrement(nextParaName, &nPNLen);
                }
                status = CLAUSESTATUS_UNKNOWN;
                break;
            case CLAUSESTATUS_BEHINDPOUND:
                if (i + nPNLen <= predicate->size && memcmp(nextParaName, predicate->ch + i, nPNLen) == 0) {
                    if (i >= sizeof (ifSpace) &&
                            memcmp(ifSpace, predicate->ch + i - sizeof (ifSpace), sizeof (ifSpace) - 1) == 0 &&
                            (i == sizeof (ifSpace) || predicate->ch[i - sizeof (ifSpace) - 1] == ' ' || predicate->ch[i - sizeof (ifSpace) - 1] == '[' || predicate->ch[i - sizeof (ifSpace) - 1] == '(') &&
                            (
                            i + nPNLen + sizeof (isNotNv) - 1 <= predicate->size &&
                            memcmp(isNotNv, predicate->ch + i + nPNLen, sizeof (isNotNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNotNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNotNv) - 1] == '[')
                            ||
                            i + nPNLen + sizeof (isNv) - 1 <= predicate->size &&
                            memcmp(isNv, predicate->ch + i + nPNLen, sizeof (isNv) - 1) == 0 &&
                            (i + nPNLen + sizeof (isNv) - 1 == predicate->size || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == ' ' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '(' || predicate->ch[i + nPNLen + sizeof (isNv) - 1] == '[')
                            )
                            ) {
                        inteN++;
                        bindIndex[0] = inteN;
                        bindType[0] = 1;
                    } else {
                        inteP++;
                        bindIndex[0] = inteP;
                        bindType[0] = 0;
                    }
                    bindIndex++;
                    bindType++;
                    SNLColNameIncrement(nextParaName, &nPNLen);
                }
                status = CLAUSESTATUS_UNKNOWN;
                break;
            default:
                switch (predicate->ch[i]) {
                    case '@':
                        status = CLAUSESTATUS_BEHINDAT;
                        break;
                    case '#':
                        status = CLAUSESTATUS_BEHINDPOUND;
                        break;
                }
                break;
        }
    }
    bindIndex[0] = 0;
    while (bindIndices[0]) {
        switch (bindTypes[0]) {
            case 1:
                bindIndices[0] += inteP + blobP;
                break;
            case 2:
                bindIndices[0] += inteP;
                break;
            case 3:
                bindIndices[0] += inteP + blobP + inteN;
                break;
        }
        bindIndices++;
        bindTypes++;
    }
}

void ConvParamIndexToParamName(size_t index, unsigned char *nextParaName, size_t *nPNLen) {
    if (index < 26) {
        nextParaName[0] = 'a' + index;
        nPNLen[0] = 1;
    } else {
        index -= 26;
        for (size_t numDig = 1;; numDig++) {
            if (index >= 26 * powsizet(10, numDig))
                index -= 26 * powsizet(10, numDig);
            else {
                nPNLen[0] = numDig + 1;
                size_t startpoint = 'a';
                for (size_t i = 0; numDig >= i; i++) {
                    nextParaName[i] = index / powsizet(10, numDig - i) + startpoint;
                    index %= powsizet(10, numDig - i);
                    startpoint = '0';
                }
                break;
            }
        }
    }
};

struct UCArr Get1stNounOfDigRelaClauParamAtIndex(unsigned char *pred, size_t index) {
    unsigned char prefixAndnPN[8] = " (#";
    size_t nPNLen;
    ConvParamIndexToParamName(index, prefixAndnPN + 3, &nPNLen);
    prefixAndnPN[3 + nPNLen] = ')';
    prefixAndnPN[4 + nPNLen] = 0;
    struct UCArr retuc;
    retuc.ch = strstr(pred, prefixAndnPN);

    for (;;) {
        retuc.ch--;
        if (retuc.ch[0] == ')') {
            do {
                retuc.ch--;
            } while (retuc.ch[0] == ' ');
        } else
            break;
    }
    retuc.size = 1;
    for (;;) {
        if (retuc.ch == pred) {
            return retuc;
        }
        retuc.ch--;
        switch (retuc.ch[0]) {
            case ' ':
            case '[':
            case '(':
                retuc.ch++;
                return retuc;
                break;
        }
        retuc.size++;
    }
}
