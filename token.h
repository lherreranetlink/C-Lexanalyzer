#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifdef _WIN32
#define LINE_FEED_COUNT 1
#else
#define LINE_FEED_COUNT 0
#endif // _WIN32

#define INITIAL_STATE -1
#define BEGIN_LOGIC_AND_STATE -2
#define BEGIN_LOGIC_OR_STATE -3
#define BEGIN_STRING_STATE -4
#define END_STRING_STATE -5
#define BEGIN_REAL_NUMBER_STATE -6
#define ERROR_STATE -7

typedef enum TokenType {IDENTIFIER, INTEGER, REAL_NUMBER, CONST_STRING, DATA_TYPE, ADD_OPERATOR, MULT_OPERATOR,
                        RELATIONAL_OPERATOR, LOGIC_OR_OPERATOR, LOGIC_AND_OPERATOR, NEGATION_OPERATOR, EQUALS_COMPARISON_OPERATOR,
                        SEMICOLON, COMA, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, LEFT_BRACE, RIGHT_BRACE, ASSIGNMENT_OPERATOR,
                         IF_KEYWORD, WHILE_KEYWORD, RETURN_KEYWORD, ELSE_KEYWORD, EOF_SIGN} TokenType;

typedef struct Token
{
    char* symbol;
    TokenType type;
}Token;

void setStateByToken(char token, int* state);
void setErrorIfExists(int* state);
Token* getNextToken(FILE* fileToAnalyze);


#endif // TOKEN_H_INCLUDED
