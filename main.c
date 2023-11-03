#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "analyzer.h"

#define MAX_TOKENS 512

void print_token(FILE* stream, Token token, const char string[]);
void print_program(FILE* stream, const ProgramNode* program, const char string[]);

int main(int argc, char* argv[]) {

    char* source = "MAIN:a=1;b=1;FIB:c=(a+b);a=b;b=c;>FIB;";
    size_t position = 0;
    size_t length = strlen(source);
    size_t count = 0;
    Token tokens[MAX_TOKENS];

    if (!tokenize(source, &position, length, tokens, &count)) {
        fprintf(stderr, "Tokenization failed: unrecognized token '%c' at position %zu\n", source[position], position);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; i++) {
        Token token = tokens[i];
        print_token(stdout, token, source);
        printf(" token of length %zu at position %zu\n", token.length, token.position);
    }

    position = 0;
    ProgramNode program;
    if (!parse_program(tokens, &position, count, &program)) {
        Token token = tokens[position];
        fprintf(stderr, "Parsing failed: unexpected token "); // TODO: expected token diagnostic
        print_token(stderr, token, source);
        fprintf(stderr, " at position %zu\n", token.position);
        return EXIT_FAILURE;
    }

    print_program(stdout, &program, source);

    AnalysisResult analysis_result = analyze_program(&program, source);
    switch (analysis_result.type) {
        case ANALYSIS_ERR_REDEFINITION: {
            const Token* label = (Token*)analysis_result.value;
            char name[MAX_NAME];
            for (size_t i = 0; i < label->length; i++) {
                name[i] = source[label->position + i];
            }
            fprintf(stderr, "Redefinition of identifier '%s' at position %zu\n", name, label->position); // TODO
            return EXIT_FAILURE;
        }
        case ANALYSIS_ERR_UNDEFINED: {
            const Token* variable = (Token*)analysis_result.value;
            char name[MAX_NAME];
            for (size_t i = 0; i < variable->length; i++) {
                name[i] = source[variable->position + i];
            }
            fprintf(stderr, "Undefined identifier '%s' at position %zu\n", name, variable->position); // TODO
            return EXIT_FAILURE;
        }
        default: break;
    }

    printf("Analyzed program OK\n");

    FILE* output = fopen("output.txt", "w");

    

    fclose(output);

    return EXIT_SUCCESS;
}

void print_token(FILE* stream, Token token, const char string[]) {
    switch (token.type) {
        case TOKEN_ARROW: fprintf(stream, "'>'"); break;
        case TOKEN_COLON: fprintf(stream, "':'"); break;
        case TOKEN_SEMICOLON: fprintf(stream, "';'"); break;
        case TOKEN_EQUALS: fprintf(stream, "'='"); break;
        case TOKEN_LPAR: fprintf(stream, "'('"); break;
        case TOKEN_RPAR: fprintf(stream, "')'"); break;
        case TOKEN_PLUS: fprintf(stream, "'+'"); break;
        case TOKEN_LABEL: {
            fputc('\'', stream);
            for (size_t i = 0; i < token.length; i++) {
                fputc(string[token.position + i], stream);
            }
            fputc('\'', stream);
            break;
        }
        case TOKEN_NUMBER: {
            fputc('\'', stream);
            for (size_t i = 0; i < token.length; i++) {
                fputc(string[token.position + i], stream);
            }
            fputc('\'', stream);
            break;
        }
        case TOKEN_VARIABLE: {
            fputc('\'', stream);
            for (size_t i = 0; i < token.length; i++) {
                fputc(string[token.position + i], stream);
            }
            fputc('\'', stream);
            break;
        }
    }
}

void print_indent(FILE* stream, size_t count) {
    for (size_t i = 0; i < count; i++) {
        fprintf(stream, "...");
    }
}

void print_value(FILE* stream, const ValueNode* value, size_t indent, const char string[]) {
    print_indent(stream, indent);
    switch (value->type) {
        case VALUE_NUMBER: {
            fprintf(stream, "NUMBER\n");
            print_indent(stream, indent + 1);
            print_token(stream, value->number, string);
            printf("\n");
            break;
        }
        case VALUE_VARIABLE: {
            fprintf(stream, "VARIABLE\n");
            print_indent(stream, indent + 1);
            print_token(stream, value->variable, string);
            printf("\n");
            break;
        }
        case VALUE_ADDITION: {
            fprintf(stream, "ADDITION\n");
            print_value(stream, value->addition.value1, indent + 1, string);
            print_value(stream, value->addition.value2, indent + 1, string);
            break;
        }
    }
}

void print_program(FILE* stream, const ProgramNode* program, const char string[]) {
    fprintf(stream, "PROGRAM\n");
    for (size_t i = 0; i < program->routine_count; i++) {
        RoutineNode* routine = &program->routines[i];
        print_indent(stream, 1);
        fprintf(stream, "ROUTINE\n");
        print_indent(stream, 2);
        fprintf(stream, "LABEL\n");
        print_indent(stream, 3);
        print_token(stream, routine->label, string);
        fprintf(stream, "\n");
        for (size_t j = 0; j < routine->action_count; j++) {

            print_indent(stream, 2);
            fprintf(stream, "ACTION\n");
            print_indent(stream, 3);

            ActionNode* action = &routine->actions[j];
            switch (action->type) {
                case ACTION_CALL: {
                    fprintf(stream, "CALL\n");
                    print_indent(stream, 4);
                    fprintf(stream, "LABEL\n");
                    print_indent(stream, 5);
                    print_token(stream, action->call.label, string);
                    break;
                }
                case ACTION_ASSIGN: {
                    fprintf(stream, "ASSIGN\n");
                    print_indent(stream, 4);
                    fprintf(stream, "VARIABLE\n");
                    print_indent(stream, 5);
                    print_token(stream, action->assign.variable, string);
                    fprintf(stream, "\n");
                    print_indent(stream, 4);
                    fprintf(stream, "VALUE\n");
                    print_value(stream, &action->assign.value, 5, string);
                    break;
                }
            }
        }
    }
    fprintf(stream, "\n");
}