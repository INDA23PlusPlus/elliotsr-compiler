#pragma once

#include <stdio.h>

#include "analyzer.h"

void generate_program(const ProgramNode* program, const char* const source, FILE* output);