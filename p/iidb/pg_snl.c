
        switch (proposition->ch[i]) {
            case '\0':
                return NULL_CHAR_FOUND;
                break;
            case ' ':
                //todo: if not a part of COMMA+SPACE, or right behind any segment, INVALID
                //todo: status = startIndependentClause OR epithetExpected OR anySegmentExpected
                if (status == START_MAIN_CLAUSE ||
                        status == START_CLAUSE_PRECEEDED_BY_COMMA_SPACE ||
                        status == START_CLAUSE_PRECEEDED_BY_DOT_SPACE ||
                        status == START_PARENTHESIS_CLAUSE_AS_NOUN ||
                        status == START_PARENTHESIS_CLAUSE_AS_CRITERIA)
                    return INVALID;
                break;
            case ',':
                //todo: if not right behind any segment, INVALID
                //todo: status = spaceExpectedBehindComma
                if (status == START_MAIN_CLAUSE ||
                        status == START_CLAUSE_PRECEEDED_BY_COMMA_SPACE ||
                        status == START_CLAUSE_PRECEEDED_BY_DOT_SPACE ||
                        status == START_PARENTHESIS_CLAUSE_AS_NOUN ||
                        status == START_PARENTHESIS_CLAUSE_AS_CRITERIA)
                    return INVALID;
                break;
            case '.':
            case '/':
            case '\\':
            case ';':
                return INVALID;
                break;
            case '(':
                //todo: if not preceeded by a noun or another (), invalid
                //todo: status = startRelativeClause
                switch (status) {
                    case START_PARENTHESIS_CLAUSE_AS_CRITERIA:
                        return INVALID;
                        break;
                    case START_MAIN_CLAUSE:
                    case START_CLAUSE_PRECEEDED_BY_COMMA_SPACE:
                    case START_CLAUSE_PRECEEDED_BY_DOT_SPACE:
                    case START_PARENTHESIS_CLAUSE_AS_NOUN:
                    case BEHIND_EPITHET:
                        status = START_PARENTHESIS_CLAUSE_AS_NOUN;
                        break;
                    case BEHIND_NOUN:
                        status = START_PARENTHESIS_CLAUSE_AS_CRITERIA;
                        break;
                    default:
                        return INVALID;
                        break;
                }
                numOfBracketOpen++;
                break;
            case ')'://todo: WRONG CODE!!!!  not behind what, it's endOfClause
                //todo: if not right behind a segment, invalid
                //todo: behindRelativeClause for potential further epithetExpected
                switch (status) {
                    case START_PARENTHESIS_CLAUSE_AS_CRITERIA:
                        return INVALID;
                        break;
                    case SNL_SENSE_SPELLING_UNKNOWNPARTOFSPEECH:
                        switch (SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNL_SENSE_SPELLING)) {
                            case NOUN_SNL_SENSE:
                                status = BEHIND_NOUN;
                                break;
                            case EPITHET_SNL_SENSE:
                                status = BEHIND_EPITHET;
                                break;
                            case SPELLING_TURNEDOUTTOBE_RAW_DIGITS:
                                status = BEHIND_RAW_DIGITS;
                                break;
                            default:
                                return INVALID;
                        }
                        break;
                    case SNL_SENSE_SPELLING_EPITHET_EXPECTED:
                        switch (SNLSpellAndRefLookup(proposition->ch + snlSpellOrRefStartInd, i - snlSpellOrRefStartInd, SNL_SENSE_SPELLING)) {
                            case EPITHET_SNL_SENSE:
                                status = BEHIND_EPITHET;
                                break;
                            case SPELLING_TURNEDOUTTOBE_RAW_DIGITS:
                                status = BEHIND_RAW_DIGITS;
                                break;
                            default:
                                return INVALID;
                        }
                        break;
                        //todo: raw
                }
                if (numOfBracketOpen == 0)
                    return INVALID;
                numOfBracketOpen--;
                break;
            case '[':
                //todo: must be at start OR behind space
                //todo: if preceeded by a noun or (), invalid
                //todo: status = startNounClause
                if (status == START_PARENTHESIS_CLAUSE_AS_CRITERIA)
                    return INVALID;
                numOfSqBrcktOpen++;
                break;
            case ']':
                //todo: if not right behind a segment, invalid
                //todo: behindNounClause for potential further epithetExpected
                if (status == START_PARENTHESIS_CLAUSE_AS_CRITERIA)
                    return INVALID;
                if (numOfSqBrcktOpen == 0)
                    return INVALID;
                numOfSqBrcktOpen--;
                break;
            case '#':
                //todo: must be at start OR behind space
                //todo: status=behindPound
                break;
            case '@':
                //todo: must be at start OR behind space
                //todo: status=behindAt
                switch (status) {
                    case START_PARENTHESIS_CLAUSE_AS_CRITERIA:
                        return INVALID;
                        break;
                    case START_MAIN_CLAUSE:
                    case START_CLAUSE_PRECEEDED_BY_COMMA_SPACE:
                    case START_CLAUSE_PRECEEDED_BY_DOT_SPACE:
                    case START_PARENTHESIS_CLAUSE_AS_NOUN:
                    case BEHIND_EPITHET:
                        status = FIRST_AT_CHAR;
                        break;
                    case BEHIND_NOUN:
                        return INVALID;
                        break;
                    case SNL_SENSE_SPELLING_UNKNOWNPARTOFSPEECH:
                    case SNL_SENSE_SPELLING_EPITHET_EXPECTED:
                        return INVALID;
                        break;

                    case FIRST_AT_CHAR:
                        status = SECOND_AT_CHAR;
                        break;
                    case SECOND_AT_CHAR:
                        return INVALID;
                        break;
                }
                break;
            default:
                //todo: if behind #, must be 1-9 or a-z, then status=statusRef
                //todo: if behind @, must be 1-9 or a-z, then status=statusRef
                //todo: ...
                switch (status) {
                    case START_PARENTHESIS_CLAUSE_AS_CRITERIA:
                        return INVALID;
                        break;
                    case START_MAIN_CLAUSE:
                    case START_CLAUSE_PRECEEDED_BY_COMMA_SPACE:
                    case START_CLAUSE_PRECEEDED_BY_DOT_SPACE:
                    case START_PARENTHESIS_CLAUSE_AS_NOUN:
                    case BEHIND_EPITHET:
                        snlSpellOrRefStartInd = i;
                        status = SNL_SENSE_SPELLING_UNKNOWNPARTOFSPEECH;
                        break;
                    case BEHIND_NOUN:
                        snlSpellOrRefStartInd = i;
                        status = SNL_SENSE_SPELLING_EPITHET_EXPECTED;
                        break;
                    case SNL_SENSE_SPELLING_UNKNOWNPARTOFSPEECH:
                    case SNL_SENSE_SPELLING_EPITHET_EXPECTED:
                        break;
                }
                break;
        }