#pragma once

#include <stdbool.h>

#include "token.h"

typedef enum ActionType {
    ACTION_CALL, ACTION_ASSIGN
} ActionType;

typedef enum ValueType {
    VALUE_NUMBER, VALUE_VARIABLE, VALUE_ADDITION
} ValueType;

typedef struct ValueNode {
    ValueType type;
    union {
        Token number;
        Token variable;
        struct {
            struct ValueNode* value1;
            struct ValueNode* value2;
        } addition;
    };
} ValueNode;

typedef struct ActionNode {
    ActionType type;
    union {
        struct {
            Token label;
        } call;
        struct {
            Token variable;
            ValueNode value;
        } assign;
    };
} ActionNode;

typedef struct RoutineNode {
    Token label;
    ActionNode* actions;
    size_t action_count;
} RoutineNode;

typedef struct ProgramNode {
    RoutineNode* routines;
    size_t routine_count;
} ProgramNode;

bool parse_program(const Token tokens[], size_t* position, size_t count, ProgramNode* result);