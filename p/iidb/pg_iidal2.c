
size_t GetNumOfVarLessThanSegIdUnderVerbSeg(sqlite3_int64 segId, sqlite3_int64 verbSegId){
    int retval;
    sqlite3_clear_bindings(selectFromAdverbialclWhereSegId);
    sqlite3_reset(selectFromAdverbialclWhereSegId);
    sqlite3_bind_int64(selectFromAdverbialclWhereSegId, 1, verbSegId);
    retval = sqlite3_step(selectFromAdverbialclWhereSegId);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        size_t sum=0;
        sqlite3_clear_bindings(selectChildrenSegFromVerbClauWhereParent);
        sqlite3_reset(selectChildrenSegFromVerbClauWhereParent);
        sqlite3_bind_int64(selectChildrenSegFromVerbClauWhereParent, 1, verbSegId);
        for(;;){
            retval = sqlite3_step(selectChildrenSegFromVerbClauWhereParent);
            //todo: check return value
            if (retval != SQLITE_ROW) {
                return sum;
            }
            sqlite3_int64 recvNounSegId = sqlite3_column_int64(selectChildrenSegFromVerbClauWhereParent, 0);
            sqlite3_int64 recvVerbSegId = sqlite3_column_int64(selectChildrenSegFromVerbClauWhereParent, 1);
            sum+=GetNumOfVarLessThanSegIdUnderNounSeg(segId,recvNounSegId)+GetNumOfVarLessThanSegIdUnderVerbSeg(segId,recvVerbSegId);
        }
    }
    return 0;
}

size_t GetNumOfVarLessThanSegIdUnderNounSeg(sqlite3_int64 segId, sqlite3_int64 nounSegId){
    if(segId==nounSegId)
        return 0;
    int retval;
    sqlite3_clear_bindings(selectFromRelativeclaWhereSegId);
    sqlite3_reset(selectFromRelativeclaWhereSegId);
    sqlite3_bind_int64(selectFromRelativeclaWhereSegId, 1, nounSegId);
    retval = sqlite3_step(selectFromRelativeclaWhereSegId);
    //todo: check return value
    if (retval == SQLITE_ROW) {
        size_t sum=0;
        sqlite3_clear_bindings(selectChildrenSegFromNounClauWhereParent);
        sqlite3_reset(selectChildrenSegFromNounClauWhereParent);
        sqlite3_bind_int64(selectChildrenSegFromNounClauWhereParent, 1, nounSegId);
        for(;;){
            retval = sqlite3_step(selectChildrenSegFromNounClauWhereParent);
            //todo: check return value
            if (retval != SQLITE_ROW) {
                return sum;
            }
            sqlite3_int64 recvNounSegId = sqlite3_column_int64(selectChildrenSegFromNounClauWhereParent, 0);
            sqlite3_int64 recvVerbSegId = sqlite3_column_int64(selectChildrenSegFromNounClauWhereParent, 1);
            sum+=GetNumOfVarLessThanSegIdUnderNounSeg(segId,recvNounSegId)+GetNumOfVarLessThanSegIdUnderVerbSeg(segId,recvVerbSegId);
        }
    }
    if (nounSegId>segId)
        return 0;
    sqlite3_clear_bindings(segExistInVar);
    sqlite3_reset(segExistInVar);
    sqlite3_bind_int64(segExistInVar, 1, nounSegId);
    retval = sqlite3_step(segExistInVar);
    //todo: check return value
    if (retval == SQLITE_ROW)
        return 1;
    else
        return 0;
}

size_t GetNumOfVarLessThanSegIdUnderTab(const unsigned char *tabName, size_t tabNameLen, sqlite3_int64 segId){
    int retval;
    sqlite3_clear_bindings(selectFromIndepclWhereTabname);
    sqlite3_reset(selectFromIndepclWhereTabname);
    sqlite3_bind_blob(selectFromIndepclWhereTabname, 1, tabName, tabNameLen, SQLITE_STATIC);
    size_t sum=0;
    for(;;){
        retval = sqlite3_step(selectFromIndepclWhereTabname);
        //todo: check return value
        if (retval != SQLITE_ROW) {
            return sum;
        }
        sum+=GetNumOfVarLessThanSegIdUnderNounSeg(segId,sqlite3_column_int64(selectFromIndepclWhereTabname, 0))+GetNumOfVarLessThanSegIdUnderVerbSeg(segId,sqlite3_column_int64(selectFromIndepclWhereTabname, 1))+GetNumOfVarLessThanSegIdUnderNounSeg(segId,sqlite3_column_int64(selectFromIndepclWhereTabname, 2));
    }
}

