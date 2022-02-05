#include "header.h"
#include "processor_header.h"

int execute(struct processor* processor)
{
    verify(processor);

    int k = 0;
    processor->ip = 0;

    while(k == 0){
        int cmd = 0;
        cmd = processor->code[processor->ip];
        printf("cmd = %d\n", cmd);
        switch(cmd)
        {
            
            #include "commands.h"

            default:
            {
                printf("default\n");
                return NO_SUCH_COMAND;
            }

        }
    }
    return ALL_OK;
}


errors_t verify(struct processor* proc)
{
    errors_t error = ALL_OK;

    if((error = stack_ok(proc->stk)) != ALL_OK)
        return error;

    if(!proc)
        return BAD_PROC_POINTER;

    if(proc->code_bin == NULL)
        return NULL_CODE_BIN;

    if(proc->code == NULL)
        return NULL_CODE;

    if(proc->ip < 0)
        return BAD_INSTRACTION_POINTER;

    if(proc->size_code < 0)
        return NEGATIVE_CODE_SIZE;

    return ALL_OK;

}

#undef CHECKSTACK
#undef DEFCMD