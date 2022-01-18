#include "enseashQ3.h"

int main(void){
	char commande[MAX];
	int pid;
	int commande_len;

	
	write(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));
	write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

	
	while(1){
		commande_len = read(STDOUT_FILENO, commande, MAX);
		commande[commande_len-1]='\0';
	
		
		if(strncmp("exit",commande,4)==0 || commande_len==0){
			write (STDOUT_FILENO, EXIT_MSG, strlen(EXIT_MSG));
			return(EXIT_SUCCESS);
		}

		pid=fork();
		if(pid==0){
			execlp(commande, commande,NULL);
			return(EXIT_FAILURE);
		}
		else{
			wait(NULL);
			write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
		
		}
	}
	
}


