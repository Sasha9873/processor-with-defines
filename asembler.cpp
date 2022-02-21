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

    hash_table* tab = create_tab(&error);

    proc.code = asembler(file_asm, code_txt, &error, &proc, tab);
    //printf("\nhash_kel = %d\n\n", str_hash("kel", tab));
    //printf("%p key = %s ip = %d\n\n", tab->table[str_hash("kel", tab)], tab->table[str_hash("kel", tab)]->key, tab->table[str_hash("kel", tab)]->ip);
    
    fclose(code_txt);
    code_txt = fopen("code.txt", "wb+");/**/
    fseek(file_asm, 0, SEEK_SET);


    proc.code = asembler(file_asm, code_txt, &error, &proc, tab);


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

int* asembler(FILE* file_asm, FILE* code_txt, errors_t* error, struct processor* proc, hash_table* tab)
{
    /*if(tab->table[str_hash("kel", tab)] != NULL){
        fprintf(code_txt, "\n111111111 %p key = %s ip = %d\n\n", tab->table[str_hash("kel", tab)], tab->table[str_hash("kel", tab)]->key, tab->table[str_hash("kel", tab)]->ip);
        printf("AAAAA %p key = %s ip = %d\n\n", tab->table[str_hash("kel", tab)], tab->table[str_hash("kel", tab)]->key, tab->table[str_hash("kel", tab)]->ip);
    }
    fprintf(code_txt, "jjbjbjbj\n");*/

    
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


    /*int labels[10];
    for(int i = 0; i < 10; i++)
        labels[i] = 0;*/

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
        int digit = 0;

        int k_str_num = sscanf(str, "%s %d %s", cmd, &value, trash);
        int k_strs = sscanf(str, "%s %s %d %s", cmd, reg, &digit, trash);
        printf("k_strs = %d k_str_num = %d %s %s %s\n", k_strs, k_str_num, cmd, reg, trash);

        #include "commands.h"
        /*else*/ if(cmd[strlen(cmd) - 1] == ':'){
            strncpy(reg, cmd, strlen(cmd) - 1); //delete ':'
            reg[strlen(cmd) - 1] = '\0';
            printf("cmd = %s, reg = %s cmd_len = %ld\n", cmd, reg, strlen(cmd));
            if(get_ip(reg, tab, error) == -6)
                add_elem(tab, reg, proc->ip);
            //labels[cmd[0] - '0'] = proc->ip;
        }
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

    return code;

}


int what_arg(int k_str_num, int k_strs, char* reg, char* key, hash_table* tab, errors_t* error)
{
    if(k_str_num == 2)
        return -5;

    if(key != NULL && tab != NULL){
        printf("ip = %d\n", get_ip(key, tab, error));
        return get_ip(key, tab, error);
    }

    if(strcmp(reg, "rax") == 0)
        return -1;
    else if(strcmp(reg, "rbx") == 0)
        return -2;
    else if(strcmp(reg, "rcx") == 0)
        return -3;
    else if(strcmp(reg, "rdx") == 0)
        return -4;
    
    return -7;

}


hash_table* create_tab(errors_t* error)
{
    hash_table* tab = (hash_table*)malloc(sizeof(hash_table));
    if(tab == NULL)
        return tab;

    tab->length = 100003;

    int index = 0;
    for(index = 0; index < tab->length; index++){
        tab->table[index] = NULL;
    }

    return tab;
}

int add_elem(hash_table* tab, char* key, int value)
{
    int hash = str_hash(key, tab);

    if(!tab->table[hash])
    {
        tab->table[hash] = (labels*)malloc(sizeof(labels));

        if(tab->table[hash] != NULL){
            tab->table[hash]->key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
            strcpy(tab->table[hash]->key, key);
            tab->table[hash]->ip = value;
            tab->table[hash]->next = NULL;
            return ALL_OK;
        }

        return NOT_MEMORY;
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


int get_ip(char* key, hash_table* tab, errors_t* error)
{
    printf("jkkj00\n");
    if(tab == NULL){
        *error = NULL_POINTER;
        printf("NULL_POINTER\n");
        return NULL_POINTER;
    }

    int hash = str_hash(key, tab);
    printf("khash = %d\n", hash);

    labels* cur;

    if(tab->table[hash] != NULL)
        cur = tab->table[hash];
    else
        return -6;

    while(strcmp(cur->key, key) != 0 && cur != NULL)
    {
        cur = cur->next;
    }

    if(cur == NULL){
        printf("NULL cur\n");
        return -6;
    }

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



int str_hash(char* str, hash_table* tab)
{
    int coeff = 17;
    int hash = 0, coeff_pow = 1;

    for (int index = 0; index < strlen(str); index++)
    {
        hash += int(str[index]) * coeff_pow;
        coeff_pow *= coeff;
        hash %= tab->length;
        coeff_pow %= tab->length;
    }
    return hash;
}

/*int listing(FILE* file_lst, int* code)
{
    //fprintf("%p ", )
}*/

#undef DEFCMD