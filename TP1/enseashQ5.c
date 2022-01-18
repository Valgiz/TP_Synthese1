#include "enseashQ5.h"



//fonction mise en en forme du prompt
int formate_prompt (int ES, char *return_code, long time_diff){
	char exit_status[MAX];
	
	sprintf(return_code, "%s",PROMPT_BEG);
	sprintf(exit_status,"[exit:%d | time:%ldms]",ES,time_diff);
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
	struct timespec t2;
	int time_diff;

	
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
		
		clock_gettime(CLOCK_MONOTONIC, &t1);
		pid=fork();
		if(pid==0){
			execlp(commande, commande,NULL);
			return(EXIT_FAILURE);
		}
		else{
			wait(&status);
			clock_gettime(CLOCK_MONOTONIC, &t2);
			if(WIFEXITED(status)){
				ES = WEXITSTATUS(status);
				time_diff= (t2.tv_nsec-t1.tv_nsec)/1000000+((long)(t2.tv_sec-t1.tv_sec)*1000) ;
				formate_prompt(ES, return_code, time_diff);
				
				write(STDOUT_FILENO, return_code, strlen(return_code));
			}
			else{
				write(STDOUT_FILENO, PROMPT_BEG, strlen(PROMPT_BEG));
				write(STDOUT_FILENO, PROMPT_END, strlen(PROMPT_END));
			}
			
		
		}
	}
	
}


