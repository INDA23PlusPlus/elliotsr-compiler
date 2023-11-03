#include <stdbool.h>
#include <stdlib.h>

#include "lexer.h"

const bool whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

const bool range(char c, char start, char end) {
    return start <= c && c <= end;
}

bool tokenize(const char string[], size_t* position, size_t length, Token tokens[], size_t* count) {

    size_t offset = *position;

    while (*position - offset < length) {
        
        if (whitespace(string[*position])) {
            (*position)++;
            continue;
        } 

        Token token;
        token.position = *position;
        if (string[*position] == ':') {
            token.type = TOKEN_COLON;
            (*position)++;
        } else if (string[*position] == ';') {
            token.type = TOKEN_SEMICOLON;
            (*position)++;
        } else if (string[*position] == '=') {
            token.type = TOKEN_EQUALS;
            (*position)++;
        } else if (string[*position] == '(') {
            token.type = TOKEN_LPAR;
            (*position)++;
        } else if (string[*position] == ')') {
            token.type = TOKEN_RPAR;
            (*position)++;
        } else if (string[*position] == '+') {
            token.type = TOKEN_PLUS;
            (*position)++;
        } else if (string[*position] == '>') {
            token.type = TOKEN_ARROW;
            (*position)++;
        } else if (range(string[*position], 'a', 'z')) {
            token.type = TOKEN_VARIABLE;
            while (*position - offset < length && range(string[*position], 'a', 'z')) {
                (*position)++;
            }
        } else if (range(string[*position], 'A', 'Z')) {
            token.type = TOKEN_LABEL;
            while (*position - offset < length && range(string[*position], 'A', 'Z')) {
                (*position)++;
            }
        } else if (range(string[*position], '0', '9')) {
            token.type = TOKEN_NUMBER;
            while (*position - offset < length && range(string[*position], '0', '9')) {
                (*position)++;
            }
        } else {
            return false;
        }

        token.length = *position - token.position;

        tokens[(*count)++] = token;
    }

    return true;
}