#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"

// CHECK THAT:
// - VARIABLES ARE DEFINED BEFORE USE
// - LABELS ARE DEFINED BEFORE JUMPS

#define MAX_LABELS 512
#define MAX_VARIABLES 512
#define MAX_NAME 64

typedef enum AnalysisResultType {
    ANALYSIS_OK,
    ANALYSIS_ERR_REDEFINITION,
    ANALYSIS_ERR_UNDEFINED
} AnalysisResultType;

typedef struct AnalysisResult {
    AnalysisResultType type;
    void* value;
} AnalysisResult;

AnalysisResult analyze_program(const ProgramNode* program, const char* const source);