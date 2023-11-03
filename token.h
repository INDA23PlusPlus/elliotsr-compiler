#pragma once

#include <stddef.h>

typedef enum TokenType {
    TOKEN_NUMBER, TOKEN_LABEL, TOKEN_VARIABLE,
    TOKEN_COLON, TOKEN_SEMICOLON, TOKEN_LPAR, TOKEN_RPAR,
    TOKEN_PLUS, TOKEN_EQUALS, TOKEN_ARROW
} TokenType;

typedef struct Token {
    TokenType type;
    size_t position, length;
} Token;