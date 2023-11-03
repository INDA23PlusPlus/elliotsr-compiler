#include "generator.h"


#define GENERATE_DEFINITION(var, value) #var "=" #value

void generate_program(const ProgramNode* program, const char* const source, FILE* output) {
    fwrite(GENERATE_DEFINITION(x, 5), sizeof(GENERATE_DEFINITION(x, 5)), 1, output);
}