#include "enseashQ1.h"

int main(void){
	write(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));
	write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
	return(EXIT_SUCCESS);
}
