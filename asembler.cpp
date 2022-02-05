#include "header.h"
#include "enum.h"
#include "asm.h"


int main()
{
    struct processor proc = {};

    FILE* file_asm = fopen("asm.txt", "rb");
    FILE* code_txt = fopen("code.txt", "wb+");
    proc.code_bin = fopen("code.bin", "wb+");
    printf("%p\n", file_asm);

    errors_t error;
    printf("%p\n", proc.code);
    proc.code = asembler(file_asm, code_txt, &error, &proc);

    printf("error = %d\n", error);

    //printf("\n \n \n%n", proc.code);
    printf("\n    %ld\n", proc.size_code);

    fclose(file_asm);
    fclose(code_txt);
    fclose(proc.code_bin);
    free(proc.code);
    proc.code = NULL;

    return 0;
}

int* asembler(FILE* file_asm, FILE* code_txt, errors_t* error, struct processor* proc)
{
    char cmd[10];
    char reg[10];

    char str[40];
    char sym = '\0';
    int capacity = 10;
    int* code = (int*)calloc(capacity, sizeof(int));
    printf("memory = %p\n", code);
    int* new_memory = NULL;
    proc->ip = 0;
    int value = 0;
    int len = 0;
    int pop_cmd = 0;

    while(!feof(file_asm) ){
        len = 0;
        if(capacity > 0){
            if(capacity <= proc->ip + 3){   //+ max length of comand + 1 (+ 3)
                if((new_memory = (int*)realloc(code, (capacity * CHANGE + 3) * sizeof(int))) == NULL){
                    *error = NOT_MEMORY;
                    return code;
                }
                else{
                    code = new_memory;
                    capacity = capacity * CHANGE + 3;
                }
            }
        }
        else{
            capacity = 10;
            int* code = (int*)calloc(capacity, sizeof(int));
        }

        if(fgets(str, 40, file_asm) == NULL)
            break;

        char trash[10];

        int k_str_num = sscanf(str, "%s %d %s", cmd, &value, trash);
        int k_strs = sscanf(str, "%s %s %s", cmd, reg, trash);
        //printf("k1 = %d k2 = %d %s %s %s\n", k1, k2, cmd, reg, trash);

        #include "commands.h"
        /*else*/{*error = BAD_COMMAND;
            return code;
        }
    }    

    code[proc->ip] = '\0';

    proc->size_code = proc->ip;

    for(len = 0; len < proc->ip; len++)
            fprintf(code_txt, "%d" ,code[len]);

    fprintf(code_txt, "\n");

    fwrite(code, sizeof(code[0]), proc->ip, proc->code_bin);

    /*printf("\n \n \ncode:%n", proc->code);

    free(code);
    code = NULL;*/

    return code;

}


int what_arg(int k_str_num, int k_strs, char* reg)
{
    if(k_str_num == 2)
        return 0;

    if(strcmp(reg, "rax") == 0)
        return 1;
    else if(strcmp(reg, "rbx") == 0)
        return 2;
    else if(strcmp(reg, "rcx") == 0)
        return 3;
    else if(strcmp(reg, "rdx") == 0)
        return 4;

    return -1;

}

/*int listing(FILE* file_lst, int* code)
{
    //fprintf("%p ", )
}*/

#undef DEFCMD