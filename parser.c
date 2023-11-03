#include <stdbool.h>
#include <stdlib.h>

#include "parser.h"

bool parse_token(TokenType type, const Token tokens[], size_t* position, size_t count, Token* result) {
    
    if (*position >= count) {
        return false;
    }

    Token token = tokens[*position];
    if (token.type == type) {
        if (result != NULL) {
            *result = token;
        }
        (*position)++;
        return true;
    }

    return false;
}

bool parse_value(const Token tokens[], size_t* position, size_t count, ValueNode* result) {

    if (*position >= count) {
        return false;
    }

    ValueNode value;

    if (parse_token(TOKEN_NUMBER, tokens, position, count, &value.number)) {
        value.type = VALUE_NUMBER;
    } else if (parse_token(TOKEN_VARIABLE, tokens, position, count, &value.variable)) {
        value.type = VALUE_VARIABLE;
    } else if (parse_token(TOKEN_LPAR, tokens, position, count, NULL)) {
        value.type = VALUE_ADDITION;
        value.addition.value1 = (ValueNode*)malloc(sizeof(ValueNode));
        if (!parse_value(tokens, position, count, value.addition.value1)) {
            return false;
        }
        if (!parse_token(TOKEN_PLUS, tokens, position, count, NULL)) {
            return false;
        }
        value.addition.value2 = (ValueNode*)malloc(sizeof(ValueNode));
        if (!parse_value(tokens, position, count, value.addition.value2)) {
            return false;
        }
        if (!parse_token(TOKEN_RPAR, tokens, position, count, NULL)) {
            return false;
        }
    } else {
        return false;
    }

    if (result != NULL) {
        *result = value;
    }

    return true;
}

bool parse_action(const Token tokens[], size_t* position, size_t count, ActionNode* result) {

    if (*position >= count) {
        return false;
    }

    ActionNode action;

    if (parse_token(TOKEN_ARROW, tokens, position, count, NULL)) {
        action.type = ACTION_CALL;
        if (!parse_token(TOKEN_LABEL, tokens, position, count, &action.call.label)) {
            return false;
        }
    } else if (parse_token(TOKEN_VARIABLE, tokens, position, count, &action.assign.variable)) {
        action.type = ACTION_ASSIGN;
        if (!parse_token(TOKEN_EQUALS, tokens, position, count, NULL)) {
            return false;
        }
        if (!parse_value(tokens, position, count, &action.assign.value)) {
            return false;
        }
    } else {
        return false;
    }

    if (!parse_token(TOKEN_SEMICOLON, tokens, position, count, NULL)) {
        return false;
    }

    if (result != NULL) {
        *result = action;
    }

    return true;
}

bool parse_routine(const Token tokens[], size_t* position, size_t count, RoutineNode* result) {

    if (*position >= count) {
        return false;
    }

    RoutineNode routine;

    if (!parse_token(TOKEN_LABEL, tokens, position, count, &routine.label)) {
        return false;
    }

    if (!parse_token(TOKEN_COLON, tokens, position, count, NULL)) {
        return false;
    }

    size_t action_capacity = 8;
    routine.action_count = 0;
    routine.actions = malloc(action_capacity * sizeof(ActionNode));
    while (parse_action(tokens, position, count, &routine.actions[routine.action_count])) {
        if (routine.action_count >= action_capacity) {
            action_capacity *= 2;
            routine.actions = realloc(routine.actions, action_capacity * sizeof(ActionNode));
        }
        routine.action_count++;
    }

    if (result != NULL) {
        *result = routine;
    }

    return true;
}

bool parse_program(const Token tokens[], size_t* position, size_t count, ProgramNode* result) {

    ProgramNode program;

    size_t routine_capacity = 8;
    program.routine_count = 0;
    program.routines = malloc(routine_capacity * sizeof(RoutineNode));
    while (parse_routine(tokens, position, count, &program.routines[program.routine_count])) {
        if (program.routine_count >= routine_capacity) {
            routine_capacity *= 2;
            program.routines = realloc(program.routines, routine_capacity * sizeof(RoutineNode));
        }
        program.routine_count++;
    }
    
    if (result != NULL) {
        *result = program;
    }

    return true;
}