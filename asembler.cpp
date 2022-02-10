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

    hash_table* tab = create_tab(error);

    int labels[10];
    for(int i = 0; i < 10; i++)
        labels[i] = 0;

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
        /*else*/ if(cmd[strlen(cmd) - 1] == ':')
            add_elem(tab, cmd, -1);//обрезать : !!!1
            labels[cmd[0] - '0'] = proc->ip;
        else{
            *error = BAD_COMMAND;
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
    for(int i = 0; i < 10; i++)
        printf("label[%d] = %d\n", i, labels[i]);

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


hash_table* create_tab()
{
    hash_table* tab = (hash_table*)malloc(sizeof(hash_table));
    if(tab == NULL)
        return tab;

    int index = 0;
    for(index = 0; index < LEN_TAB; index++){
        tab->table[index] = NULL;
    }

    return tab;
}

int add_elem(hash_table* tab, char* key, int value)
{
    int hash = str_hash(key);
    if(!tab->table[hash])
    {
        tab->table[hash] = (labels*)malloc(sizeof(labels));
        strcpy(tab->table[hash]->key, key);
        tab->table[hash]->ip = value;
        tab->table[hash]->next = NULL;
        return ALL_OK;
    }

    labels* ptr = tab->table[hash];
    while(ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    labels* new_label = (labels*)malloc(sizeof(labels));
    if(!new_label)
    {
        return NOT_MEMORY;
    }

    strcpy(new_label->key, key);
    new_label->ip = value;
    new_label->next = NULL;
    ptr->next = new_label;

    return ALL_OK;
}


void write_hash(hash_table* tab)
{
    for(int i = 0; i < tab->length; i++)
    {
        printf("")
        labels* ptr = tab->table[i];
        while(ptr != NULL)
        {
            printf("   key = %s ip = %d", ptr->key, ptr->ip);
            ptr = ptr->next;
        }

        printf("\n");
    }
}
/**/


int get_ip(char* key, hash_table* tab)
{
    int hash = str_hash(key);

    labels* cur = tab->table[hash];

    while(strcmp(cur->key, key) != 0 && cur != NULL)
    {
        cur = cur->next;
    }

    if(cur == NULL)
        return -1;

    return cur->ip;
}

void clear_hash(hash_table* tab)
{
    for(int i = 0; i < tab->length; i++)
    {
        labels* ptr = tab->table[i];
        while(ptr != NULL)
        {
            labels* old_ptr = ptr;
            ptr = ptr->next;
            free(old_ptr);
        }
    }
}



int str_hash(char* str)
{
    const int coeff = 17;
    int hash = 0, coeff_pow = 1;
    for (int index = 0; index < strlen(str); index++)
    {
        hash += str[index] * coeff_pow;
        coeff_pow *= coeff;

        hash %= LEN_LABEL;
        coeff_pow %= LEN_LABEL;
    }
    return hash;
}

/*int listing(FILE* file_lst, int* code)
{
    //fprintf("%p ", )
}*/

#undef DEFCMD