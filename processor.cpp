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



int processor_dump(struct processor* proc, errors_t reason)
{
    /*if(reason != 0)
        fprintf(proc->file_with_errors, "Dump was called because %s(%d)\n", error_names[abs(reason)], reason);

    fprintf(proc->file_with_errors, "Processor[%p]", proc);

    errors_t error = verify(proc);
    if(error == ALL_OK)
        fprintf(proc->file_with_errors, "(ok)\n");
    else
        fprintf(proc->file_with_errors, "ERROR %d %s", error, error_names[abs(error)]);

    fprintf(proc->file_with_errors, "{\n");

    fprintf(proc->file_with_errors, "   Regs[%p] = %p (pointer):\n", &proc->regs, proc->regs);
    fprintf(proc->file_with_errors, "      regs[0] = %d   ;rax\n      regs[1] = %d   ;rbx\n", proc->regs[0], proc->regs[1]);
    fprintf(proc->file_with_errors, "      regs[2] = %d   ;rcx\n      regs[3] = %d   ;rcx\n", proc->regs[2], proc->regs[3]);

    fprintf(proc->file_with_errors, "   Flags[%p] = %p (pointer):\n", &proc->flag, proc->flag);
    fprintf(proc->file_with_errors, "      zero_flag = %d\n      size_flag = %d\n", proc->flag->zero_flag, proc->flag->size_flag);
    fprintf(proc->file_with_errors, "      overfloat_flag = %d\n", proc->flag->overfloat_flag);


    fprintf(proc->file_with_errors, "   code[%p] = ", proc->code);
    for(int i = 0; i < proc->size_code; ++i)
        fprintf(proc->file_with_errors, "%d", proc->code[i]);

    fprintf(proc->file_with_errors, "\n   size_code = %ld\n", proc->size_code);
    fprintf(proc->file_with_errors, "   ip = %d\n", proc->ip);

    fprintf(proc->file_with_errors, "   code_bin[%p] = %p\n", &proc->code_bin, proc->code_bin);

    fprintf(proc->file_with_errors, "   file_with_errors[%p] = %p\n", &proc->file_with_errors, proc->file_with_errors);

    fprintf(proc->file_with_errors, "}\n\n\n");*/

   return 0;
}


#undef CHECKSTACK
#undef CHECKPROC
#undef DEFCMD