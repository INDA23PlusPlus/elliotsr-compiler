#pragma once

#include <stdbool.h>

#include "token.h"

bool tokenize(const char string[], size_t* position, size_t length, Token tokens[], size_t* count);