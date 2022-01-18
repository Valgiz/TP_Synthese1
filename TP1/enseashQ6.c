#include "enseashQ6.h"



//fonction mise en en forme du prompt
int formate_prompt (int ES, char *return_code, long time_diff){
	char exit_status[MAX];
	
	sprintf(return_code, "%s",PROMPT_BEG);
	sprintf(exit_status,"[exit:%d | time:%ldms]",ES,time_diff);
	strcat(return_code,exit_status);
	strcat(return_code,PROMPT_END);
	
	return(EXIT_SUCCESS);
}

//on compte les espaces dans la commande pour avoir le nombre d argument
int nb_argument(char *commande){
	int i=0;
	int nb_space=0;
	while(commande[i] != '\0'){
		if(commande[i] == ' '){
			nb_space++;
		}
		i++;
	}
	return (nb_space+1);
}




int main(void){
	char commande[MAX];
	int commande_len;
	int status;
	int pid;
	int i=0;
	
	char return_code[MAX];
	int ES;
	
	int argc;
	char **argv;
	char *arg;
	
	struct timespec t1;
	struct timespec t2;
	long time_diff;

	//message d'accueil
	write(STDOUT_FILENO, MESSAGE, strlen(MESSAGE));
	write(STDOUT_FILENO, PROMPT_BEG, strlen(PROMPT_BEG));
	write(STDOUT_FILENO, PROMPT_END, strlen(PROMPT_END));
	
	while(1){
		commande_len = read(STDOUT_FILENO, commande, MAX);
		commande[commande_len-1]='\0';
		
		//commande_len=0 correspond a un ctrl+d pour quitter la shell
		if(strncmp("exit",commande,4)==0 || commande_len==0){
			write (STDOUT_FILENO, EXIT_MSG, strlen(EXIT_MSG));
			return(EXIT_SUCCESS);
		}
		
		//allocation de la mémoire, c'est surement la partie qui fait buger le programme, mais je n'ai pas trouvé pourquoi. je pense que 
		//la mémoire n'est pas réalloué a chaque itération et donc est free plusieur fois mais je ne suis pas sur et ne sais pas comment le réparer
		argc = nb_argument(commande);
		argv = malloc(argc*sizeof(char*));
		arg = strtok(commande, " ");
		while(arg !=NULL){
			
			argv[i]=malloc(sizeof(arg));
			strcpy(argv[i],arg);
			i++;
			arg = strtok(NULL, " ");
			
		}
		argv[i]=NULL;
		
		//on commence a compter le temps
		clock_gettime(CLOCK_MONOTONIC, &t1);
		pid=fork(); // on fork pour que le fils exécute le programme, tandis que le père fasse tourner la shell
		if(pid==0){
			
			execvp(argv[0], argv);
			return(EXIT_FAILURE);
		}
		else{
			wait(&status);  //on récupère les informations sur l'execution du fils
			clock_gettime(CLOCK_MONOTONIC, &t2); //on mesure a nouveau le temps, puis on le compare a la mesure précédente pour la durée d'execution
			if(WIFEXITED(status)){
				ES = WEXITSTATUS(status);
				time_diff= ((long)((t2.tv_sec-t1.tv_sec)*1000)) + (t2.tv_nsec-t1.tv_nsec)/1000000;
				formate_prompt(ES, return_code, time_diff);
				
				write(STDOUT_FILENO, return_code, strlen(return_code));
			}
			else{
				write(STDOUT_FILENO, PROMPT_BEG, strlen(PROMPT_BEG));
				write(STDOUT_FILENO, PROMPT_END, strlen(PROMPT_END));
			}
		}
		//on libère la mémoire
		for(i=argc-1; i>-1; i--){
			argv[i] = NULL;
			free(argv[i]);
		}
		free(argv);
	}
	
}


