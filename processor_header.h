#pragma once

#include <cmath>

#include "header.h"
#include "enum.h"

#define CHECKSTACK(reason)\
    if(stack_ok(stack1)){\
        fprintf(stack1->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
        stack_dump(stack1, reason);\
    }\
    else{\
        fprintf(stack1->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
        stack_dump(stack1, reason);\
    }

#define CHECKPROC(reason)\
    if(verify(processor)){\
        fprintf(processor->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
        processor_dump(processor, reason);\
    }\
    else{\
        fprintf(processor->file_with_errors, "Called from %s() at %s(%d),\n", __FUNCTION__, __FILE__, __LINE__);\
        processor_dump(processor, reason);\
    }

#define DEFCMD(name, num, arg, ...)\
    case CMD_##name:{\
        __VA_ARGS__\
        processor->ip += arg + 1;\
        CHECKPROC(ALL_OK);\
        break;\
    }\


static const char *error_names[] = {
    "All is ok",
    "Not enough memory",
    "Two ctors",
    "Not pointer to error",
    "Bad stack pointer",
    "Negative capacity of the stack",
    "Negative size of the stack",
    "Wrong begin stack canary",
    "Wrong end stack canary",
    "Wrong begin data canary",
    "Wrong end data canary",
    "Bad data pointer",
    "Capacity smaller then size",    //12
    "Wrong hash",
    "This command does not exist",
    "Pop in empty stack",
    "You wrote not a number",       //16
    "Wrong name of the register",
    "Bad struct processor pointer",
    "NULL pointer to file with bin code",
    "NULL pointer to massif with code", //20
    "Instraction pointer smaller than 0",
    "Negative size of the code",
    "Null pointer",
};

typedef enum errors{ALL_OK                  = 0,
                    NOT_MEMORY              = -1,
                    TWO_CTOR                = -2,
                    NOT_POINT_ERROR         = -3,
                    STACK_POINTER           = -4,
                    NEGATIVE_CAPASITY       = -5,
                    NEGATIVE_SIZE           = -6,
                    BEGIN_CANARY            = -7,
                    END_CANARY              = -8,
                    DATA_BEGIN_CANARY       = -9,
                    DATA_END_CANARY         = -10,
                    DATA_POINTER            = -11,
                    CAP_SMALLER_SIZE        = -12,
                    WRONG_HASH              = -13,
                    NO_SUCH_COMAND          = -14,
                    POP_EMPTY_STACK         = -15,
                    WRONG_VALUE             = -16,
                    BAD_REG                 = -17,
                    BAD_PROC_POINTER        = -18,
                    NULL_CODE_BIN           = -19,
                    NULL_CODE               = -20,
                    BAD_INSTRACTION_POINTER = -21,
                    NEGATIVE_CODE_SIZE      = -22,
                    NULL_POINTER            = -23}
errors_t;

int stack_push(Stack* stack1, int value);
int stack_pop(Stack* stack1);
int stack_ctor(Stack* stack1);
int stack_dtor(Stack* stack1);
errors_t stack_ok(Stack* stack1);    //enum errors stack_ok
int stack_dump(Stack* stack1, errors_t reason = ALL_OK);
int stack_hash(Stack* stack1, errors_t reason = ALL_OK);
int stack_print(Stack* stack1);

int execute(processor* processor);
errors_t verify(processor* processor);
int processor_dump(processor* proc, errors_t reason);

int processor(Stack* stack1, processor* processor);
