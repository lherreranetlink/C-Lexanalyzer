#include "token.h"

Token* getNextToken(FILE* fileToAnalyze)
{
    char token[BUFSIZ], *tokenp, c;
    int currentState = INITIAL_STATE, continueStateLoop = 1;
    static int filePos = 0;
    Token* newToken = NULL;
    tokenp = token;
    fseek(fileToAnalyze, filePos, SEEK_SET);
    while (!feof(fileToAnalyze))
    {
        while (continueStateLoop)
        {
            if (currentState == INITIAL_STATE)
            {
                for (c = fgetc(fileToAnalyze); isspace(c) && c != EOF ; c = fgetc(fileToAnalyze), filePos++)
                    if (c == '\n')
                        filePos += LINE_FEED_COUNT;
            }
            else
                c = fgetc(fileToAnalyze);
            switch(currentState)
            {
            case INITIAL_STATE:
            {
                filePos++;
                *tokenp++ = c;
                if(isdigit(c))
                {
                    currentState = INTEGER;
                    break;
                }
                if(isalpha(c) || c == '_')
                {
                    currentState = IDENTIFIER;
                    break;
                }
                setStateByToken(c, &currentState);
            }
            break;
            case ADD_OPERATOR:
            case MULT_OPERATOR:
            case LEFT_PARENTHESIS:
            case RIGHT_PARENTHESIS:
            case LEFT_BRACE:
            case RIGHT_BRACE:
            case SEMICOLON:
            case COMA:
                continueStateLoop = 0;
                break;
            case RELATIONAL_OPERATOR:
            case NEGATION_OPERATOR:
            case ASSIGNMENT_OPERATOR:
            {
                if (c == '=')
                {
                    currentState = (currentState == NEGATION_OPERATOR || currentState == ASSIGNMENT_OPERATOR)?EQUALS_COMPARISON_OPERATOR:currentState;
                    *tokenp++ = c;
                    filePos++;
                    break;
                }
                continueStateLoop = 0;
            }
            break;
            case BEGIN_LOGIC_AND_STATE:
            {
                if (c == '&')
                {
                    *tokenp++ = c;
                    currentState = LOGIC_AND_OPERATOR;
                    filePos++;
                    break;
                }
                continueStateLoop = 0;
            }
            break;
            case LOGIC_AND_OPERATOR:
                continueStateLoop = 0;
                break;
            case BEGIN_LOGIC_OR_STATE:
            {
                if (c == '|')
                {
                    *tokenp++ = c;
                    currentState = LOGIC_OR_OPERATOR;
                    filePos++;
                    break;
                }
                continueStateLoop = 0;
            }
            case LOGIC_OR_OPERATOR:
                continueStateLoop = 0;
                break;
            case BEGIN_STRING_STATE:
            {
                tokenp--;
                filePos++;
                if (c != '"')
                {
                    *tokenp++= c;
                    currentState = CONST_STRING;
                    break;
                }
                continueStateLoop = 0;
            }
            break;
            case CONST_STRING:
            {
                if (c != '"' && c != '\n' && c != EOF)
                {
                    *tokenp++ = c;
                    filePos++;
                    break;
                } else if (c == '"')
                {
                    currentState = END_STRING_STATE;
                    filePos++;
                }
                continueStateLoop = 0;
            }
            break;
            case BEGIN_REAL_NUMBER_STATE:
            {
                if (isdigit(c))
                {
                    *tokenp++ = c;
                    currentState = REAL_NUMBER;
                    filePos++;
                    break;
                }
                continueStateLoop = 0;
            }
            break;
            case REAL_NUMBER:
            {
                if (isdigit(c))
                {
                    *tokenp++ = c;
                    filePos++;
                    break;
                }
                continueStateLoop = 0;
            }
            break;
            case INTEGER:
            {
                if(c == '.')
                {
                    *tokenp++ = c;
                    currentState = BEGIN_REAL_NUMBER_STATE;
                    filePos++;
                    break;
                }
                else if (isdigit(c))
                {
                    *tokenp++ = c;
                    filePos++;
                    break;
                }
                continueStateLoop = 0;
            }
            break;
            case IDENTIFIER:
            {
                if(isalnum(c) || c =='_')
                {
                    *tokenp++ = c;
                    filePos++;
                    break;
                }
                continueStateLoop = 0;
            }
            break;
            case ERROR_STATE:
                continueStateLoop = 0;
                break;
            case EOF_SIGN:
                filePos = 0;
                tokenp = token;
                *tokenp++ = '$';
                continueStateLoop = 0;
            }
        }
        setErrorIfExists(&currentState);
        *tokenp = '\0';
        newToken = (Token*)malloc(sizeof(Token));
        newToken->symbol = (char*)malloc(sizeof(token));
        strcpy(newToken->symbol, token);
        newToken->type = currentState;
        break;
    }
    return newToken;
}

void setStateByToken(char token, int *state)
{
    switch(token)
    {
    case '+':
    case '-':
        *state = ADD_OPERATOR;
        break;
    case '*':
    case '/':
        *state = MULT_OPERATOR;
        break;
    case '<':
    case '>':
        *state = RELATIONAL_OPERATOR;
        break;
    case '!':
        *state = NEGATION_OPERATOR;
        break;
    case '=':
        *state = ASSIGNMENT_OPERATOR;
        break;
    case '&':
        *state = BEGIN_LOGIC_AND_STATE;
        break;
    case '|':
        *state = BEGIN_LOGIC_OR_STATE;
        break;
    case '(':
        *state = LEFT_PARENTHESIS;
        break;
    case ')':
        *state = RIGHT_PARENTHESIS;
        break;
    case '{':
        *state = LEFT_BRACE;
        break;
    case '}':
        *state = RIGHT_BRACE;
        break;
    case ';':
        *state = SEMICOLON;
        break;
    case ',':
        *state = COMA;
        break;
    case '"':
        *state = BEGIN_STRING_STATE;
        break;
    case EOF:
        *state = EOF_SIGN;
        break;
    default:
        *state = ERROR_STATE;
    }
}

void setErrorIfExists(int* state)
{
    switch(*state)
    {
    case BEGIN_REAL_NUMBER_STATE:
    case BEGIN_LOGIC_AND_STATE:
    case BEGIN_LOGIC_OR_STATE:
    case BEGIN_STRING_STATE:
    case CONST_STRING:
        *state = ERROR_STATE;
    }
}
