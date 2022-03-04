#include <stddef.h>
#include <math.h>/**/

typedef enum errors{
    ALL_OK       =  0,
    NOT_MEMORY   = -1,
    BAD_REG      = -2,
    BAD_ENTRY    = -3,
    BAD_COMMAND  = -4,
    NULL_POINTER = -5,
}errors_t;


typedef struct labels{
    char* key;
    int ip;
    struct labels* next;
}labels;

typedef struct{
    int length = 100003;
    labels* table[100003];
}hash_table;


int add_elem(hash_table* tab, char* key, int value);
hash_table* create_tab(errors_t* error);
void clear_hash(hash_table* tab);
int str_hash(char* str, hash_table* tab);
int get_ip(char* key, hash_table* tab, errors_t* error);


int* asembler(FILE* file_asm, FILE* code_txt, errors_t* error, struct processor* proc, hash_table* tab);
int listing(FILE* file_lst, int* code);

/**
 * returns -5             if it is a number
 *         -1, -2, -3, -4 if it is a register rax, rbx, rcx, rdx respectively 
 *         ip(code[ip])   if the command was jump
 *         -7             if it is a fault.
 */
int what_arg(int k_str_num, int k_strs, char* reg, char* key, hash_table* tab, errors_t* error);

/**
 * 
 */
int init_arg(int* code, int arg_is, FILE* code_txt, errors_t* error, struct processor* proc, char* name, int num);


#define DEFCMD(name, num, arg, ...)\
if (strcmp(cmd, #name) == 0){\
    code[proc->ip] = num;\
    if(arg == 1){\
        int arg_is = 0;\
        if(num >= 7 && num <= 13 || num == 6)\
            arg_is = what_arg(k_str_num, k_strs, reg, reg, tab, error);\
        else\
            arg_is = what_arg(k_str_num, k_strs, reg, NULL, NULL, error);\
        \
        if(arg_is == -7){\
            printf("arg_is = %d\n", arg_is);\
            *error = BAD_ENTRY;\
            return code;\
        }\
        else if(arg_is == -5){\
            proc->ip++;\
            code[proc->ip] = value;\
            printf("%d\n", code[proc->ip-1]);fflush(stdin);\
            fprintf(code_txt,"%04d   %02x %02x\t ", proc->ip - 1, num, value);\
            fprintf(code_txt,"%s %d\n", #name, value);\
        }\
        else if(arg_is == -1 || arg_is == -2 || arg_is == -3 || arg_is == -4){\
            proc->ip++;\
            printf("%d ", code[proc->ip-1]);fflush(stdin);\
            code[proc->ip] = abs(arg_is);\
            printf("%d\n", code[proc->ip]);fflush(stdin);\
            fprintf(code_txt,"%04d   %02x ", proc->ip - 1, num);\
            fprintf(code_txt,"%02x\t %s ", code[proc->ip], #name);\
            fprintf(code_txt,"%d\n", code[proc->ip]);\
        }\
        else{\
            proc->ip++;\
            if(arg_is == -6){\
                code[proc->ip] = -1;printf("aaaaaaa\n");}\
            else{\
                code[proc->ip] = arg_is;printf("17 arg_is = %d\n", arg_is);}\
            fprintf(code_txt,"%04d   %02x ", proc->ip - 1, num);\
            fprintf(code_txt,"%02x\t %s ", code[proc->ip], #name);\
            fprintf(code_txt,"%d\n", code[proc->ip]);\
        }\
    }\
    else if(arg == 2){\
        printf("k_strs = %d, name = %s\n", k_strs, #name); fflush(stdin);\
        if(k_strs != 3){\
            printf("k_strs = %d, name = %s\n", k_strs, #name);\
            *error = BAD_ENTRY;\
            return code;\
        }\
        char new_reg[10];\
        strncpy(new_reg, reg, strlen(reg) - 1);\
        new_reg[strlen(reg) - 1] = '\0';\
        int arg_is = what_arg(k_str_num, k_strs, new_reg, NULL, NULL, error);\
        if(arg_is == -1 || arg_is == -2 || arg_is == -3 || arg_is == -4){\
            proc->ip++;\
            printf("reg = %s arg_is = %d\n", new_reg, arg_is);\
            printf("%d ", code[proc->ip-1]);fflush(stdin);\
            code[proc->ip] = abs(arg_is);\
            proc->ip++;\
            code[proc->ip] = digit;\
            printf("%d\n", code[proc->ip]);fflush(stdin);\
            fprintf(code_txt,"%04d   %02x ", proc->ip - 1, num);\
            fprintf(code_txt,"%02x %02d\t %s ", code[proc->ip - 1], digit, #name);\
            fprintf(code_txt,"%d %d\n", code[proc->ip - 1], digit);\
        }\
        else{\
            printf("k_strs = %d, name = %s\n", k_strs, #name);\
            *error = BAD_REG;\
            return code;\
        }\
    }\
    else if(k_strs != 1){\
        printf("k_strs = %d, name = %s\n", k_strs, #name);\
        *error = BAD_ENTRY;\
        return code;\
    }\
    else{\
        fprintf(code_txt,"%04d   %02x\t\t %s\n", proc->ip, num, #name);\
    }\
    proc->ip++;\
}\
else\

