#include "analyzer.h"
#include "hash.h"

bool labels[MAX_LABELS];
bool variables[MAX_LABELS];

AnalysisResult analyze_value(const ValueNode* value, const char* const source) {
    switch (value->type) {
        case VALUE_NUMBER: {
            break;
        }
        case VALUE_VARIABLE: {
            const Token* variable = &value->variable;
            unsigned char name[MAX_NAME];
            size_t i;
            for (i = 0; i < variable->length; i++) {
                name[i] = source[variable->position + i];
            }
            name[i] = '\0';
            unsigned long h = hash(name) % MAX_VARIABLES;
            if (variables[h] != true) { // variable not defined
                return (AnalysisResult) {
                    .type = ANALYSIS_ERR_UNDEFINED,
                    .value = (void*)variable
                };
            }
            break;
        }
        case VALUE_ADDITION: {
            AnalysisResult result1 = analyze_value(value->addition.value1, source);
            if (result1.type != ANALYSIS_OK) {
                return result1;
            }
            AnalysisResult result2 = analyze_value(value->addition.value2, source);
            if (result2.type != ANALYSIS_OK) {
                return result2;
            }
            break;
        }
        default: break;
    }
    return (AnalysisResult) {
        .type = ANALYSIS_OK,
        .value = NULL
    };
}

AnalysisResult analyze_action(const ActionNode* action, const char* const source) {

    switch (action->type) {
        case ACTION_ASSIGN: {
            const ValueNode* value = &action->assign.value;
            AnalysisResult value_result = analyze_value(value, source);
            if (value_result.type != ANALYSIS_OK) {
                return value_result;
            }
            const Token* variable = &action->assign.variable;
            unsigned char name[MAX_NAME];
            size_t i;
            for (i = 0; i < variable->length; i++) {
                name[i] = source[variable->position + i];
            }
            name[i] = '\0';
            unsigned long h = hash(name) % MAX_VARIABLES;
            variables[h] = true; // define variable
            break;
        }
        case ACTION_CALL: {
            break;
        }
    }

    return (AnalysisResult) {
        .type = ANALYSIS_OK,
        .value = NULL
    };
}

AnalysisResult analyze_routine(const RoutineNode* routine, const char* const source) {

    unsigned char label[MAX_NAME];
    size_t i;
    for (i = 0; i < routine->label.length; i++) {
        label[i] = source[routine->label.position + i];
    }
    label[i] = '\0';
    unsigned long h = hash(label) % MAX_LABELS;
    if (labels[h] == true) return (AnalysisResult) { // label already defined
        .type = ANALYSIS_ERR_REDEFINITION,
        .value = (void*)&routine->label
    }; 
    labels[h] = true;

    for (size_t i = 0; i < routine->action_count; i++) {
        const ActionNode* action = &routine->actions[i];
        AnalysisResult action_result = analyze_action(action, source);
        if (action_result.type != ANALYSIS_OK) {
            return action_result;
        }
    }

    return (AnalysisResult) {
        .type = ANALYSIS_OK,
        .value = NULL
    };
}

AnalysisResult analyze_program(const ProgramNode* program, const char* const source) {
    for (size_t i = 0; i < program->routine_count; i++) {
        const RoutineNode* routine = &program->routines[i];
        AnalysisResult routine_result = analyze_routine(routine, source);
        if (routine_result.type != ANALYSIS_OK) {
            return routine_result;
        }
    }
    return (AnalysisResult) {
        .type = ANALYSIS_OK,
        .value = NULL
    };
}