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
#include"../utils/indexio.c"
#include "../utils/hash.c"


#define MAXSTRINGS 1000
#define MAXCHAR 100

int totalcount = 0;
int lowestcount = 0;

void printarrstr(char *strarr[]){
	for (int i=0; strarr[i] != NULL; i++){
		printf("string in element %d is: %s\n", i, strarr[i]);
	}

}


/*                                                                                                                                                               
 * int / int (only free struct)                                                                                                                                  
 */                                                                                                                                                              
void delete_keycount(void* doccount_v){                                                                                                                          
  doccount_i* doccount = (doccount_i*) doccount_v;
                                                                                                                                                                 
  free(doccount);                                                                                                                                                
}


void delete_wordqueue(void *wqueue_v){                                                                                                                           
  wqueue_i* wqueuehash = (wqueue_i*) wqueue_v;                                                                                                                   
  free(wqueuehash->word);                                                                                                                                       
                                                                                                                                                                 
  qapply(wqueuehash->queue, delete_keycount);                                                                                                                    
  qclose(wqueuehash->queue);                                                                                                                                     
  free(wqueuehash);                                                                                                                                              
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
		//		printf("freed string in element %d: %s\n", i, strarr[i]);
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

bool checkAndOr(char *word){
	if ((strcmp(word, "or")==0) || (strcmp(word, "and")==0)){
		return true;
	}
	return false; 
}

/*
 *
 */
void getCount(void* doccount_v){
	doccount_i* doccount = (doccount_i*) doccount_v;
	
	totalcount = doccount->count + totalcount;


}

static bool wqsearchfn(void* element, const void *key){                                                                                                          
  wqueue_i* count_w;                                                                                                                                             
  count_w = (wqueue_i*) element;                                                                                                                                 
  char *word;                                                                                                                                                    
  word = (char*) key;                                                                                                                                            
  if (count_w->word == NULL){                                                                                                                                    
    return false;                                                                                                                                                
  }                                                                                                                                                              
                                                                                                                                                                 
  if (strcmp(count_w->word, word)==0){                                                                                                                           
    //printf("found true\n");                                                                                                                                    
    return true;                                                                                                                                                 
  }                                                                                                                                                              
  else{                                                                                                                                                          
     //printf("found false\n");                                                                                                                                  
    return false;                                                                                                                                               
  }                                                                                                                                                                                                                                                                                                                              
}            

void findWordandCount(hashtable_t* hash, char *word){
	
  wqueue_i* found = (wqueue_i*) hsearch(hash, wqsearchfn, word, strlen(word));
	if (found !=NULL){
	qapply(found->queue, getCount);
	}
	else{
		return;
	}
	//printf("total count for word %s: %d", word, count);
	
}

void updateLowestCount(){
	printf("total count: %d vs lowest count %d\n", totalcount, lowestcount);
	if (totalcount < lowestcount){
		lowestcount = totalcount;
			printf("lowestcount %d and total count %d\n", lowestcount, totalcount);	 
	}
	else{
		return;
	}

	
	
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
	bool firstword = true;
	//word = (char*) malloc(MAXCHAR * sizeof(char));
	//	strarr = (char*) malloc(((MAXSTRINGS * MAXCHAR) * sizeof(char)));
	char result[MAXSTRINGS];
	
	hashtable_t* hash;
	hash = indexload("indexnm2", "pages-depth3");
	
 
	// load index into hash
	
	for (int i = 0; i < MAXSTRINGS; i++)
		{
			strarr[i]=NULL;
		}
	
	while (loop){
		printf(">");
		str = (char*) malloc(MAXSTRINGS * sizeof(char));

		//scan entire string 
		scanf("%[^\n]%c", str, &newline);
		
		printf("str: %s\n", str);
		memset(tempstr, '\0', sizeof(tempstr));

		//get first word of string 
		word = strtok(str, " \t");
		printf("word within string: %s\n", word);
		firstword = true;
		
		while (word != NULL){
			
			//check conditions
			if (checkEnd(word)==true){
				printf("program terminated\n");
				loop=false;
				//	free(word);
				//printarrstr(strarr);
				break;
			}
			if (checkAlpha(word)){
				printf("invalid query because characters not in alphabet\n");
				//free(word);
				isalpha = true; 
			}
			if (checkLength(word)){
				printf("invalid query because string less than 3 characters\n");
				//free(word);
				islength = true;
			}
			
			normalizeWord(word);

			//exclude and, or, non alphabets, short words 
			// if all conditions are good, put counts with each word 
			if (!checkAndOr(word) && !isalpha && !islength){
				strcpy(tempword, word);
				strcat(tempword, " ");

				// update totalcount for each word
				findWordandCount(hash, word);
				printf("totalcount for word %s: %d\n", word, totalcount);
				// update count for the specific word if not first time through
				// if first time through lowest count = total count for the word

				if (firstword){
					lowestcount = totalcount;
					}
				else{
				updateLowestCount();
				}
				printf("lowest count after updating word %s: %d\n", word, lowestcount); 
				
				
				// build string 
				strcat(tempstr, tempword);
				strcat(tempstr, ": ");
				
				// convert count to str type and concact to tempstr
				char countstr[2];
				sprintf(countstr, "%d", totalcount);
				strcat(tempstr, countstr);
				strcat(tempstr, " ");
				
				//reset counts
				totalcount = 0;
				

			}
			// get next word
			word = strtok(NULL, " \t");
			printf("word within string: %s\n", word);

			//reset and update checks
			isalpha = false;
			islength = false;
			loop = true;
			firstword= false;
			//updateLowestCount();  
		}
		// now combine tempstr into str and put into str array
		// this is when the program still needs to run but finished with one query 
		if (!isalpha && !islength && loop){
			char lcountstr[2];
			sprintf(lcountstr, "%d", lowestcount);
			strcat(tempstr, "- ");
			strcat(tempstr, lcountstr);

			// put str into strarr at position i
			strcpy(str, tempstr);
			strarr[i]=str;
			printf("%s\n", str);

			//reset lowest count
			lowestcount = 0;

			// move i up 1 for string array position update  
			i++;			 
			
		}
		
		//this is when eof is called 
		else{
			printarrstr(strarr);
			free(str);
		}
			
	}

	freestrarr(strarr);
	happly(hash, delete_wordqueue);     
	
	hclose(hash);
}
		
	
	//free(word);

	
	
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
		
