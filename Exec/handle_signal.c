
#include "../minishell.h"

void    ctrl_slash_handler(int signum)
{
	(void)signum;
//	if (STATUS)
//	{
//		printf("Quit (core dumped)\n");
//		kill(STATUS, SIGTERM);
//	}
}

void    ctrl_c_handler(int signum)
{
//	printf("s = %d\n", STATUS);
	(void)signum;
//	if (STATUS)
//	{
//		kill(STATUS, SIGTERM);
		printf("\n");
//	}
//	else
//	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
//	}
}


int	define_signals()
{
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, ctrl_slash_handler);
	return (0);
}
