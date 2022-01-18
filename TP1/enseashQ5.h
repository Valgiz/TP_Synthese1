#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include "stdio.h"



#define MESSAGE "Bienvenue dans le shell ENSEA. \nPour quitter, tapper 'exit'.\n"
#define PROMPT_BEG "enseash "
#define PROMPT_END "% "
#define EXIT_MSG "bye bye...\n"
#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1
#define MAX 256


int formate_prompt (int ES, char *return_code, long time_diff);


