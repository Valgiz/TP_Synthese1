#include "enseashQ4.h"



//fonction mise en en forme du prompt
int formate_prompt (int ES, char *return_code){
	char exit_status[MAX];
	
	sprintf(return_code, "%s",PROMPT_BEG);
	sprintf(exit_status,"[exit:%d]",ES);
	strcat(return_code,exit_status);
	strcat(return_code,PROMPT_END);
	
	return(EXIT_SUCCESS);
}




int main(void){
	char commande[MAX];
	int commande_len;
	int status;
	int pid;
	//char exit_status[MAX];
	char return_code[MAX];
	int ES;
	
	struct timespec t1;

	
	write(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));

	write(STDOUT_FILENO, PROMPT_BEG, strlen(PROMPT_BEG));
	write(STDOUT_FILENO, PROMPT_END, strlen(PROMPT_END));
	
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
			wait(&status);
			clock_gettime(CLOCK_MONOTONIC, &t1);
			if(WIFEXITED(status)){
				ES = WEXITSTATUS(status);
				
				formate_prompt(ES, return_code);
				
				write(STDOUT_FILENO, return_code, strlen(return_code));
			}
			else{
				write(STDOUT_FILENO, PROMPT_BEG, strlen(PROMPT_BEG));
				write(STDOUT_FILENO, PROMPT_END, strlen(PROMPT_END));
			}
			
		
		}
	}
	
}


