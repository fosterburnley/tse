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


#define MAXSTRINGS 100
#define MAXCHAR 100

int totalcount = 0;
int lowestcount = 0;
//int id = 0;


typedef struct rankIDstruct{
	int rank;
	int id;
	char *url;

}rankid_t;



void printrankid(void* rankid_v){
	rankid_t* rankid = (rankid_t*) rankid_v;
	int rank = rankid->rank;
	int id = rankid->id;
	char *url = "";
	url = rankid->url;
	printf("rank: %d, id: %d, url: %s\n", rank, id, url);

}



/*
 *
 */
void printarrstr(char *strarr[]){
	for (int i=0; strarr[i] != NULL; i++){
		printf("word in element %d is: %s\n", i, strarr[i]);
	}

}
/*

void print_doccount(void *doccount_v){                                                                                                                                                           
  //printf("hello");                                                                                                                                                                             
  doccount_i* doccount= (doccount_i*)(doccount_v);                                                                                                                                               
  printf("id: %d, count: %d\n\n", doccount-> id, doccount->count);                                                                                                                               
                                                                                                                                                                                                 
}                                                                                                                                                                                                

void print_hash(void *wqueue_v){                                                                                                                                                                 
                                                                                                                                                                                                 
  wqueue_i* wqueuehash = (wqueue_i*) wqueue_v;                                                                                                                                                   
                                                                                                                                                                                                
  printf("word in hash: %s\n", wqueuehash->word);                                                                                                                                                
  //  print_doccount((void*)(wqueue->queue));                                                                                                                                                    
  qapply(wqueuehash->queue, print_doccount);                                                                                                                                                     
  //printf("count: %d\n", count_w->count);                                                                                                                                                       
}
*/

/*                                                                                                                                                               
 * int / int (only free struct)                                                                                                                                  
 */                                                                                                                                                              
void delete_keycount(void* doccount_v){                                                                                                                          
  doccount_i* doccount = (doccount_i*) doccount_v;
                                                                                                                                                                 
  free(doccount);                                                                                                                                                
}

/*
 *
 */
void delete_wordqueue(void *wqueue_v){                                                                                                                  
  wqueue_i* wqueuehash = (wqueue_i*) wqueue_v;                                                                                                            
  free(wqueuehash->word);                                                                                                                                 
                                                                                                                                                          
  qapply(wqueuehash->queue, delete_keycount);                                                                                                             
  qclose(wqueuehash->queue);                                                                                                                             
  free(wqueuehash);                                                                                                                                      
}

/*
 *
 */
void delete_rankid(void *rankid_v){
	rankid_t* rankid = (rankid_t*) rankid_v;
	free(rankid->url);
	free(rankid);
}

/*
 *
 */
void freeword(char *str){
	char* word; 
	word = strtok(str, " ");
	while (word != NULL){
		free(word);
		word = strtok(str, " ");
	}

}

/*
 *
 */
void freestrarr(char *strarr[]){
	for (int i = 0; strarr[i] !=NULL; i++){
		//		printf("freed string in element %d: %s\n", i, strarr[i]);
		//freeword(strarr[i]);
		free(strarr[i]);
	}
	
}

/*
 *
 */
bool checkEnd(char *word){

	if ((strcmp(word, "EOF")==0) || (strcmp(word, "eof")==0)){
		return true;
	}
	return false;
}

/*
 *
 */
bool checkLength(char *word){
	if(strlen(word) < 3){
		return true;
	}
	return false; 

}

/*
 *
 */
bool checkAlpha(char *word){
	for (int i = 0; i < strlen(word); i++){
		if (isalpha(word[i]) == 0){
			return true;
		}
	}
	return false; 
		
}

/*
 *
 */
int normalizeWord(char *word){

	for (int i = 0; i < strlen(word); i++){
		word[i]=tolower(word[i]);
	}
	return 0;
}

/*
 *
 */
bool checkAndOr(char *word){
	if ((strcmp(word, "or")==0) || (strcmp(word, "and")==0)){
		return true;
	}
	return false; 
}

/*
 *

void getCountID(void* doccount_v){
	doccount_i* doccount = (doccount_i*) doccount_v;
	
	totalcount = doccount->count + totalcount;
	id = doccount->id;

}
*/

/*
 *
 */
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

static bool searchid (void*element, const void *key){
	doccount_i* doccount = (doccount_i*) element;
	int* id = (int*)key;
	if (doccount->id != *id){
		return false;
	}
	else{
		return true;
	}

	
}
/*
 *
 */
void findWordandCount(hashtable_i* hash, char *word, int id){
	
  wqueue_i* found = (wqueue_i*) hsearch(hash, wqsearchfn, (void*)word, strlen(word));
	if (found !=NULL){

		// if word is found in hash, update total count for specific docid
		doccount_i* founddoc = (doccount_i*) qsearch(found->queue, searchid, (void*)&id);
		if (founddoc!=NULL){
			totalcount = founddoc->count + totalcount;
		}
		// if word is not in doc 
		else{
			totalcount = 0;
		}
	}
	// if word is not found in hash 
	else{
		return;
	}
	//printf("total count for word %s: %d", word, count);
	
}

/*
 *
 */
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

/*
* extract url from saved page in pages-depth3
* return url
*/
char* getURL(int id){
	FILE *fp;
	char pathandfile[MAXCHAR];
	char *url = (char*) malloc(MAXSTRINGS*sizeof(char));
	
	sprintf(pathandfile, "../%s/%d", "pages-depth3", id);
	//	printf("looking for file in %s\n", pathandfile);
	fp = fopen(pathandfile, "r");
	if (fp == NULL){
		printf("warning: can't find file to get URL for query rank");
		return NULL;
	}
	
	fscanf(fp, "%s", url);  
	//	printf("url from id %d: %s \n", id, url);
	fclose(fp);
	return url;

}

void initstrarr(char *strarr[]){
	for (int i = 0; i < MAXSTRINGS; i++){
		strarr[i]=NULL; 
	}
}


void resetstrarr(char *strarr[]){

	int i = 0;
	while(strarr[i] != NULL){
		memset(strarr[i], '\0', MAXSTRINGS*sizeof(char));
		i++;
	}


}
void printdocrank(char *tempstr, char *str, int lowestcount){

	 char lcountstr[2];                                                                                                                                                                                    
	 sprintf(lcountstr, "%d", lowestcount);                                                                                                                                                               
	 strcat(tempstr, "- ");                                                                                                                                                                               
	 strcat(tempstr, lcountstr);                                                                                                                                                                          
	 // put str into strarr at position i                                                                                                                                                                 
	 strcpy(str, tempstr);                                                                                                                                                                                
	 printf("each doc string %s\n", str); 
	 memset(tempstr, '\0', sizeof(tempstr)); 
}


int main(){

	//position in string array 
	

	// structures to handle strings 
	char *strarr[MAXSTRINGS];
	char tempstr[MAXCHAR];
	char tempword[MAXCHAR];
	char *word;
	char *str; 
	char newline;

	//checks
	bool loop = true;
	bool isalpha = false;
	bool islength = false;
	//	bool firstword = true;
	char result[MAXSTRINGS];

	// load index into hash
	hashtable_i* hash;
	hash = indexload("indexnm1", "pages-depth3");
	happly(hash, print_hash);
	// open up queue of rankids
 
	queue_t* qrankid;
	qrankid = qopen();
	//initialize string array                                                                                                                                                                
	initstrarr(strarr);  
	// while user does not enter eof 
	while (loop){
		
		int i = 0;	
		printf(">");
		str = (char*) malloc(MAXSTRINGS * sizeof(char));
		
		//scan entire string 
		scanf("%[^\n]%c", str, &newline);
		
		printf("str: %s\n", str);
		memset(tempstr, '\0', sizeof(MAXSTRINGS*sizeof(char)));
		
		//get first word of string 
		word = strtok(str, " \t");
		printf("word within string: %s\n", word);
		//firstword = true;
		
		while (word != NULL){
			//resetstrarr(strarr);
			//freestrarr(strarr);
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
				//store each word in strarr
				strarr[i] = word;
				strcpy(tempword, word);
				strcat(tempword, " ");
				
				// build string 
				strcat(tempstr, tempword);
				strcat(tempstr, ": ");
				
				// convert count to str type and concact to tempstr
				char countstr[2];
				sprintf(countstr, "%d", totalcount);
				strcat(tempstr, countstr);
				strcat(tempstr, " ");
				
				//reset count
				
				//update strarr position for next word
				i++;
			}
			// get next word
			word = strtok(NULL, " \t");
			printf("word within string: %s\n", word);

			//reset and update checks
			isalpha = false;
			islength = false;
			loop = true;
			//			firstword= false;
			//updateLowestCount();
			printf("printing words in str arr: query...\n");
			printarrstr(strarr);
			//free(str);
		}

		// after going through one string
		// go through words in str arr and update ranks
		
		if (!isalpha && !islength && loop){
			
			int id = 1;	
			// put in ranking
			// for all documents 
			while(id < 82){
				
				//posiiton of strarr to get word
				int j = 0;
				// for each word in query
				while (strarr[j] != NULL){
					// find count for word at id i
					findWordandCount(hash, strarr[j], id);                                                                                                                                                      
					printf("totalcount for word %s at id %d: %d\n", strarr[j], id, totalcount);                                                                                                                            
					// update count for the specific word if not first time through                                                                                                                   
					// if first time through lowest count = total count for the word                                                                                                                   
					if (j==0){                                                                                                                                                                    
          lowestcount = totalcount;                                                                                                                                                          
					}                                                                                                                                                                                 
					else{                                                                                                                                                                              
						updateLowestCount();                                                                                                                                                             
					}                                                                                                                                                                                 
					printf("lowest count after updating word %s: %d\n", strarr[j], lowestcount); 
					j++;
					//reset total count
					totalcount = 0;
				}
				//reset count                                                                                                                                                                         
				
			
				rankid_t* rankid = (rankid_t*)malloc(sizeof(rankid_t));
				
				// fill in rankid
				rankid->rank = lowestcount;
				rankid->id = id;
				//printf("rankid rank: %d, id %d\n", rankid->rank, rankid->id);
				
				// fill in url for rankid
				char *url;
				url = getURL(id);
				rankid->url = url;
				//printf("rankurl from id %d: %s\n", id, rankid->url);
				
				//print rankid
				printrankid((void*)rankid);
				qput(qrankid, rankid);
				
				//reset lowest count 
				lowestcount = 0;
				
				// move i up 1 for string array position update  
				id++;
			
			}
			printf("printing words query...\n");                                                                                                                                                                 
      printarrstr(strarr); 
			free(str);
		}
		
		//this is when eof is called
		// free usused eof string
		else{
			
			printf("printing queue of rankids...\n");
			qapply(qrankid, printrankid);
			free(str);
			
		}
	}
	
	// free hash and string array and queue of rankids
	qapply(qrankid, delete_rankid);
	qclose(qrankid);
	//freestrarr(strarr);
	happly(hash, delete_wordqueue);    
	hclose(hash);
	
}
		
