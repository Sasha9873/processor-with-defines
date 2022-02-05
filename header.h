#pragma once

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>

#define CHECK 0

#if CHECK >= 2
    #define STACK_USE_HASH
    #define STACK_USE_CANARY
    #define DATA_USE_CANARY
#elif CHECK >= 1
    #define STACK_USE_CANARY
    #define DATA_USE_CANARY
#endif // CHECK

static int CHANGE = 2;
static const int POISON = 666;
static int* POINTER_13 = (int*)13;
static const int CANARY_VALUE = 0xBADDED;
static int previous_hash_value;
static int hash_value;

/*typedef enum comands{
    OUT   = 0,
    HLT   = 1,
    PUSH  = 17,
    POP   = 18,
    ADD   = 2,
    SUB   = 3,
    MUL   = 4,
    IN    = 5,
    RPOP  = 33,
    RPUSH = 34,
}comands_t;

static const char *comands_names[] = {
        "out",
        "hlt",
        "add",
        "sub",
        "mul",
        "in",   //5
        "",
        "",
        "",
        "",
        "",
        "",     //11
        "",
        "",
        "",
        "",
        "",
        "push", //17
        "pop",
};*/

struct Stack
{
    #ifdef STACK_USE_CANARY
    int begin_canary = CANARY_VALUE;
    #endif

    size_t capacity = 3; //требуемый размер массива
    size_t current_size = 0;//текущий размер массива
    int *data = POINTER_13; //адрес массива data[capacity]

    int pop_change = 2;
    int push_change = 2;

    FILE* file_with_errors = NULL;

    #ifdef STACK_USE_CANARY
    int end_canary = CANARY_VALUE;
    #endif
};


typedef struct processor{
    struct Stack* stk;
    int regs[4];
    int* code;
    int ip = 0;
    long int size_code = 0;
    FILE* code_bin = NULL;
}Processor;


