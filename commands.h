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

DEFCMD(JA, 7, 1,{
	int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);

	if(b > a)
		processor->ip = processor->code[processor->ip + 1];
	else
		processor->ip++;
})

DEFCMD(JAE, 8, 1,{
	int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);

	if(b >= a)
		processor->ip = processor->code[processor->ip + 1];
	else
		processor->ip++;
})

DEFCMD(JB, 9, 1,{
	int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);

	if(b < a)
		processor->ip = processor->code[processor->ip + 1];
	else
		processor->ip++;
})

DEFCMD(JBE, 10, 1,{
	int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);

	if(b <= a)
		processor->ip = processor->code[processor->ip + 1];
	else
		processor->ip++;
})

DEFCMD(JE, 11, 1,{
	int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);

	if(b == a)
		processor->ip = processor->code[processor->ip + 1];
	else
		processor->ip++;
})

DEFCMD(JNE, 12, 1,{
	int a = stack_pop(processor->stk);
	int b = stack_pop(processor->stk);

	if(b != a)
		processor->ip = processor->code[processor->ip + 1];
	else
		processor->ip++;
})

DEFCMD(JMP, 13, 1,{
	processor->ip = processor->code[processor->ip + 1];
})