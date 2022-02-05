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