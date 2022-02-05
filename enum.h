#define DEFCMD(name, num, arg,...)\
	CMD_##name = num,

enum comands{
	#include "commands.h"
};

#undef DEFCMD