DEFCMD(OUT, 0, 0,{
	stack_print(processor->stk);
})

DEFCMD(HLT, 1, 0,{
	k++;
})

DEFCMD(PUSH, 17, 1,{
	int val = processor->code[processor->ip + 1];
    stack_push(processor->stk, val);
})

DEFCMD(POP, 18, 0,{
	int del = stack_pop(processor->stk);
})

DEFCMD(ADD, 2, 0,{
	int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);
	stack_push(processor->stk, a + b);
})

DEFCMD(SUB, 3, 0,{
	int b = stack_pop(processor->stk);
	int a = stack_pop(processor->stk);
	stack_push(processor->stk, a - b);
})

DEFCMD(MUL, 4, 0,{
	int a = stack_pop(processor->stk), b = stack_pop(processor->stk);
	stack_push(processor->stk, a * b);
})

DEFCMD(IN, 5, 0,{
	int val = 0;
    printf("Write value:\n");
    if(scanf("%d", &val) != 1)
        return WRONG_VALUE;
	printf("val = %d\n", val);
    stack_push(processor->stk, val);
})

DEFCMD(RPOP, 33, 1,{
	processor->regs[processor->code[processor->ip + 1] - 1] = stack_pop(processor->stk);
})

DEFCMD(RPUSH, 34, 1,{
	stack_push(processor->stk, processor->regs[processor->code[processor->ip + 1] - 1]);
})

DEFCMD(CMP, 25, 2,{
	printf("a = %d b = %d", processor->regs[processor->code[processor->ip + 1] - 1], processor->code[processor->ip + 2]);
	fflush(stdin);
	int a = processor->regs[processor->code[processor->ip + 1] - 1];
	int b = processor->code[processor->ip + 2];
	int c = a - b;

	processor->flag->zero_flag = 0;
	processor->flag->size_flag = 0;
	processor->flag->overfloat_flag = 0;

	if(c == 0)
	{
		processor->flag->zero_flag = 1;
	}
	else if(c < 0)
	{
		processor->flag->size_flag = 1;
	}
	if((a > 0) != (b > 0))             //overfloat can be only if signs are not the same 
	{								   
		if((a > 0) && (c < b))       //if a and b > 0 and it is overfloat sum will be < 0
			processor->flag->overfloat_flag = 1;
		if((a < 0) && (c > b))	   //if a and b < 0 and it is overfloat sum will be > 0
			processor->flag->overfloat_flag = 1;
	}

	printf("\nCMD zero_flag = %d, size_flag = %d overfloat_flag = %d\n", processor->flag->zero_flag, processor->flag->size_flag, processor->flag->overfloat_flag);
	fflush(stdin);
})

DEFCMD(JA, 7, 1,{
	/*int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);*/

	if((processor->flag->size_flag == 0) && (processor->flag->overfloat_flag == 0)) //a > b, c = a - b > 0
		processor->ip = processor->code[processor->ip + 1] - 2; // -2 due to processor->ip += arg + 1 in "processor_header"
	else if((processor->flag->size_flag == 1) && (processor->flag->overfloat_flag == 1))
		processor->ip = processor->code[processor->ip + 1] - 2;
})

DEFCMD(JAE, 8, 1,{
	/*int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);*/

	if((processor->flag->size_flag == 0) && (processor->flag->overfloat_flag == 0)) //a >= b, c = a - b >= 0
		processor->ip = processor->code[processor->ip + 1] - 2;
	else if((processor->flag->size_flag == 1) && (processor->flag->overfloat_flag == 1))
		processor->ip = processor->code[processor->ip + 1] - 2;
	else if(processor->flag->zero_flag == 1)
		processor->ip = processor->code[processor->ip + 1] - 2;
})

DEFCMD(JB, 9, 1,{
	/*int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);*/

	if((processor->flag->size_flag == 1) && (processor->flag->overfloat_flag == 0)) //a < b, c = a - b < 0
		processor->ip = processor->code[processor->ip + 1] - 2;
	else if((processor->flag->size_flag == 0) && (processor->flag->overfloat_flag == 1))
		processor->ip = processor->code[processor->ip + 1] - 2;
})

DEFCMD(JBE, 10, 1,{
	/*int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);*/

	if((processor->flag->size_flag == 1) && (processor->flag->overfloat_flag == 0)) //a <= b, c = a - b <= 0
		processor->ip = processor->code[processor->ip + 1] - 2;
	else if((processor->flag->size_flag == 0) && (processor->flag->overfloat_flag == 1))
		processor->ip = processor->code[processor->ip + 1] - 2;
	else if(processor->flag->zero_flag == 1)
		processor->ip = processor->code[processor->ip + 1] - 2;
})

DEFCMD(JE, 11, 1,{
	/*int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);*/

	if(processor->flag->zero_flag == 1)  // a == b
		processor->ip = processor->code[processor->ip + 1] - 2;
})

DEFCMD(JNE, 12, 1,{
	/*int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);*/

	if(processor->flag->zero_flag == 0)  // a != b
		processor->ip = processor->code[processor->ip + 1] - 2;
})

DEFCMD(JMP, 13, 1,{
	processor->ip = processor->code[processor->ip + 1] - 2;
})