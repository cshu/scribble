enum snlSpellLookupResult{
    snlSpellLookupResult_DIGITS,
    snlSpellLookupResult_ABBRNOUN_OF_CON,
    snlSpellLookupResult_ABBRNOUN_OF_ABS,
    snlSpellLookupResult_SUBNOUN_OF_CON,
    snlSpellLookupResult_SUBNOUN_OF_ABS,
    snlSpellLookupResult_CON,
    snlSpellLookupResult_ABS,
    snlSpellLookupResult_BYTNOUN,
    snlSpellLookupResult_DIGNOUN,
    snlSpellLookupResult_EPITHET,
    snlSpellLookupResult_SUBEPIT,
    snlSpellLookupResult_NONEXISTENT
};

//todo: delete
enum snlSpellAndRefLookupResult{
    SNLSPELLANDREFLOOKUPRESULT_NONEXISTENT,
    SNLSPELLANDREFLOOKUPRESULT_EPITHET,
    SNLSPELLANDREFLOOKUPRESULT_NOUN
};

//todo: delete
enum snlSpellAndRefLookupType{
    SNLSPELLANDREFLOOKUPTYPE_SPELL,
    SNLSPELLANDREFLOOKUPTYPE_REF
};

//todo: delete
enum parsePropositionResult{
    PARSEPROPOSITIONRESULT_VALIDPROP,
    PARSEPROPOSITIONRESULT_VALIDPRED,
    PARSEPROPOSITIONRESULT_INVALID
};