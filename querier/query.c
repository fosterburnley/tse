/* query.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Sun Oct 31 17:46:04 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <ctype.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>

#define MAXSTRINGS 1000
#define MAXCHAR 100


void printarrstr(char *strarr[]){
	for (int i=0; strarr[i] != NULL; i++){
		printf("string in element %d is: %s\n", i, strarr[i]);
	}

}


void freeword(char *str){
	char* word; 
	word = strtok(str, " ");
	while (word != NULL){
		free(word);
		word = strtok(str, " ");
	}

}
void freestrarr(char *strarr[]){
	for (int i = 0; strarr[i] !=NULL; i++){
		printf("freed string in element %d: %s\n", i, strarr[i]);
		//freeword(strarr[i]);
		free(strarr[i]);
	}
	
}

bool checkEnd(char *word){

	if ((strcmp(word, "EOF")==0) || (strcmp(word, "eof")==0)){
		return true;
	}
	return false;
}

bool checkLength(char *word){
	if(strlen(word) < 3){
		return true;
	}
	return false; 

}

bool checkAlpha(char *word){
	for (int i = 0; i < strlen(word); i++){
		if (isalpha(word[i]) == 0){
			return true;
		}
	}
	return false; 
		
}

int normalizeWord(char *word){

	for (int i = 0; i < strlen(word); i++){
		word[i]=tolower(word[i]);
	}
	return 0;
}



int main(){
	bool loop = true;
	int i = 0;
	char *strarr[MAXSTRINGS];
	char tempstr[MAXCHAR];
	char tempword[MAXCHAR];
	char *word;
	char *str; 
	char newline;
	bool isalpha = false;
	bool islength = false;
	word = (char*) malloc(MAXCHAR * sizeof(char));
	//	strarr = (char*) malloc(((MAXSTRINGS * MAXCHAR) * sizeof(char))); 
	while (loop){
		printf(">");
		str = (char*) malloc(MAXSTRINGS * sizeof(char));    
		scanf("%[^\n]%c", str, &newline);
		printf("str: %s\n", str);
		memset(tempstr, '\0', sizeof(tempstr));
		word = strtok(str, " ");
		printf("word within string: %s\n", word);
		//		if (word ==NULL){
		//	free(word);
		//}
		while (word != NULL){
			if (checkEnd(word)==true){
				printf("program terminated\n");
				loop=false;
				//		free(word);
				//printarrstr(strarr);
				break;
			}
			if (checkAlpha(word)){
				printf("invalid query because characters not in alphabet\n");
				//free(word);
				isalpha = true;
				break; 
			}
			if (checkLength(word)){
				printf("invalid query because string less than 3 characters\n");
				//free(word);
				islength = true;
				break;
			}
			if (!isalpha || !islength || loop){
				normalizeWord(word);
				printf("tempstr: %s\n", tempstr);
				strcpy(tempword, word);
				strcat(tempword, " ");
				printf("tempword: %s\n", tempword);
				strcat(tempstr, tempword);
				printf("tempstr: %s\n", tempstr);
				word = strtok(NULL, " ");
				printf("word within string: %s\n", word);
				//				str = (char*)realloc(str, 30*sizeof(char));
				//reset 
				isalpha = false;
				islength = false;
				loop = true;
			}
		}
		// continue 
		if (!isalpha && !islength && loop){
			strcpy(str, tempstr);
			strarr[i]=str;
			printarrstr(strarr); 
			i++;
			//free(word);
		}
		//stop
		else{
			printarrstr(strarr);
			free(word);
			//free(str);
		}
		
	}
	//free(word);
	freestrarr(strarr);
	
}
		// while there are more words in string
		/*
		while (word !=NULL){
			if ((checkEnd(word)==true)|| (checkAlpha(word)==true) || (checkLength(word)==true)) {
				if (checkEnd(word)){
					printf("program terminated\n");
					loop=false;
					free(word);
					printarrstr(strarr);
					break; 
				}
				if (checkAlpha(word)){
					printf("invalid query because characters not in alphabet\n");
					free(word);
					break;
				}
				if (checkLength(word)){
					printf("invalid query because string less than 3 characters\n");
					free(word);
					break;
				}			
			}
			normalizeWord(word);
			//strcat(tempstr, tempword);
			//			str = (char*) malloc(sizeof(char));    
			//strcpy(str, tempstr);
			//printf("str: %s\n", str);  
			//update tempword
			word = strtok(NULL, " ");
			printf("word within string: %s\n", word);  
		}
		//str = (char*) malloc(sizeof(char));
		//strcpy(str, tempstr);
		strarr[i]=str;
		//printf("strarr %d is %s\n", i, strarr[i]);
		//		printarrstr(strarr);                                                                                                    
		i++;
		*/
		//		}
		
