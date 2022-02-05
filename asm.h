typedef enum errors{
    ALL_OK      =  0,
    NOT_MEMORY  = -1,
    BAD_REG     = -2,
    BAD_ENTRY   = -3,
    BAD_COMMAND = -4,
}errors_t;


int* asembler(FILE* file_asm, FILE* code_txt, errors_t* error, struct processor* proc);
int listing(FILE* file_lst, int* code);

/**
 * returns 0          if it is a number
 *         1, 2, 3, 4 if it is a register rax, rbx, rcx, rdx respectively 
 *        -1          if it is a fault.
 */
int what_arg(int k_str_num, int k_strs, char* reg);

/**
 * 
 */
int init_arg(int* code, int arg_is, FILE* code_txt, errors_t* error, struct processor* proc, char* name, int num);


#define DEFCMD(name, num, arg, ...)\
if (strcmp(cmd, #name) == 0){\
    code[proc->ip] = num;\
    if(arg){\
        int arg_is = what_arg(k_str_num, k_strs, reg);\
        \
        if(arg_is == -1){\
            printf("arg_is = %d\n", arg_is);\
            *error = BAD_ENTRY;\
            return code;\
        }\
        else if(arg_is == 0){\
            proc->ip++;\
            code[proc->ip] = value;\
            printf("%d\n", code[proc->ip-1]);\
            fprintf(code_txt,"%04d   %02x %02x\t ", proc->ip - 1, num, value);\
            fprintf(code_txt,"%s %d\n", #name, value);\
        }\
        else{\
            proc->ip++;\
            printf("%d ", code[proc->ip-1]);\
            code[proc->ip] = arg_is;\
            printf("%d\n", code[proc->ip]);\
            fprintf(code_txt,"%04d   %02x ", proc->ip - 1, num);\
            fprintf(code_txt,"%02x\t %s ", code[proc->ip], #name);\
            fprintf(code_txt,"%d\n", code[proc->ip]);\
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

