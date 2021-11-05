/* query.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Sun Oct 31 17:46:04 2021 (-0400)
 * Version: 
 * 
 * Description: query
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
#define MAXINTS 50
int totalwordcount = 0;
int lowestcount = 0;
//int id = 0;
FILE *inputquery;                                                                                                                                 
  FILE *outputquery;
bool quiet = false; 
/*
 * struct to carry the rank, id, and url 
 */
typedef struct rankIDstruct{
	int rank;
	int id;
	char *url;

}rankid_t;


/*
 * print rankid struct 
 */

void printrankid(void* rankid_v){
	rankid_t* rankid = (rankid_t*) rankid_v;
	int rank = rankid->rank;
	int id = rankid->id;
	char *url = "";
	url = rankid->url;
	printf("rank: %d, id: %d, url: %s\n", rank, id, url);
}

void fprintrankid(void* rankid_v){                                                                                                                 
  rankid_t* rankid = (rankid_t*) rankid_v;                                                                                                         
  int rank = rankid->rank;                                                                                                                         
  int id = rankid->id;                                                                                                                             
  char *url = "";                                                                                                                                  
  url = rankid->url;                                                                                                                               
  fprintf(outputquery, "rank: %d, id: %d, url: %s\n", rank, id, url);                                                                                            
}  


void printcountTracker(int countTracker[]){

	for (int i = 0; i < MAXINTS; i++){
		printf("count at position %d: %d\n", i, countTracker[i]);
	}

}
/*
 * print words in the string array from query 
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
 * delete word and queue in word queue strcut in hash
 */
void delete_wordqueue(void *wqueue_v){                                                                                                                  
  wqueue_i* wqueuehash = (wqueue_i*) wqueue_v;                                                                                                            
  free(wqueuehash->word);                                                                                                                                 
                                                                                                                                                          
  qapply(wqueuehash->queue, delete_keycount);                                                                                                             
  qclose(wqueuehash->queue);                                                                                                                             
  free(wqueuehash);                                                                                                                                      
}

/*
 * delete the rankid struct 
 */
void delete_rankid(void *rankid_v){
	rankid_t* rankid = (rankid_t*) rankid_v;
	free(rankid->url);
	free(rankid);
}

/*
 * not used 
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
 * free each word in the string array
 */
void freestrarr(char *strarr[]){
	for (int i = 0; strarr[i] !=NULL; i++){
		//		printf("freed string in element %d: %s\n", i, strarr[i]);
		//freeword(strarr[i]);
		free(strarr[i]);
	}
	
}

/*
 * check if word = eof or EOF
 */
bool checkEnd(char *word){

	if ((strcmp(word, "EOF")==0) || (strcmp(word, "eof")==0)){
		return true;
	}
	return false;
}

/*
 * check for each word's length
 */
bool checkLength(char *word){
	if(strlen(word) < 3){
		return true;
	}
	return false; 

}

/*
 * check if each word is a actual word
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
 * lowercase the words
 */
int normalizeWord(char *word){

	for (int i = 0; i < strlen(word); i++){
		word[i]=tolower(word[i]);
	}
	return 0;
}

/*
 * check for and /or words
 * if "and", return -1
 * if none, return 0
 * if "or", return 1
 */
int checkAndOr(char *word){
	if ((strcmp(word, "or"))==0){
		return 1;
	}
	else if ((strcmp(word, "and"))==0){
		return -1;
	}
	else{
		return 0;
	}
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
 * search function for hash for word 
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

/*
 * search function for id in each queue of doccount structs
 */
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
 * find the word and update count for each id corresponding to the word 
 */
void findWordandCount(hashtable_i* hash, char *word, int id){
	
  wqueue_i* found = (wqueue_i*) hsearch(hash, wqsearchfn, (void*)word, strlen(word));
	if (found !=NULL){

		// if word is found in hash, update total count for specific docid
		doccount_i* founddoc = (doccount_i*) qsearch(found->queue, searchid, (void*)&id);
		if (founddoc!=NULL){
			totalwordcount = founddoc->count + totalwordcount;
		}
		// if word is not in doc 
		else{
			totalwordcount = 0;
		}
	}
	// if word is not found in hash 
	else{
		return;
	}
	//printf("total count for word %s: %d", word, count);
	
}

/*
 * update the lowest count when total count < lowest count
 */
void updateLowestCount(){
	//	printf("total count: %d vs lowest count %d\n", totalcount, lowestcount);
	if (totalwordcount < lowestcount){
		lowestcount = totalwordcount;
		//printf("lowestcount %d and total count %d\n", lowestcount, totalcount);	 
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

/*
 * initialize the strarray
 */
void initstrarr(char *strarr[]){
	for (int i = 0; i < MAXSTRINGS; i++){
		strarr[i]=NULL; 
	}
}

void initcountTracker(int countTracker[]){
	for (int i = 0; i < MAXINTS; i++){
		countTracker[i] = -1;
	}
}
/*
 * reset the string array
 */
void resetstrarr(char *strarr[]){

	int i = 0;
	while(strarr[i]!=NULL){
		memset(strarr[i], '\0', MAXSTRINGS*sizeof(char));
		i++;
	}


}
void printstatement(char string[]){
	//	printf(quiet ? "true\n" : "false\n");
	if (quiet){
		fprintf(outputquery, "%s", string);
	}
	else{
		printf("%s", string);
	}
	
}
/*
 * not used 
 */
void printdocrank(char *tempstr, char *str, int lowestcount){

	 char lcountstr[2];                                                                                                                                                                                    
	 sprintf(lcountstr, "%d", lowestcount);                                                                                                                                                               
	 strcat(tempstr, "- ");                                                                                                                                                                               
	 strcat(tempstr, lcountstr);                                                                                                                                                                          
	 // put str into strarr at position i                                                                                                                                                                 
	 strcpy(str, tempstr);                                                                                                                                                                                
	 printf("each doc string %s\n", str); 
	 // memset(tempstr, '\0', sizeof(tempstr)); 
}


int main(int argc, char *argv[]){
	//int main(){
	
	// load int variables filename and path name to read from 
	char *filename = argv[2];                                                                                                                       
  char *pagedir = argv[1];


	// handle flag case 
	char ipathandfile[MAXSTRINGS];
	char opathandfile[MAXSTRINGS];

	// if # of arguments is less than 4
	if (argc < 3 || argc > 6){                                                                                                                       
    printf("wrong number of arguments: usage error: <pagedir> <indexnm> [-q] < myqueries.txt > myoutput\n");                                                                 
		exit(EXIT_FAILURE);                                                                                                                            
  }
	

	// if number of arguments is 4 or greater and either flag doesn't equal to -q or there aren't 6 arguments
	if (argc >=4){
		char *flag = argv[3];
		quiet = true;
		printf("flag: %s\n", flag);
		if ((strcmp(flag, "-q")!=0) || (argc != 6)){
			printf("wrong flag use:usage error: <pagedir> <indexnm> [-q] < myqueries.txt > myoutput\n");
			exit(EXIT_FAILURE);
		}
	// get output and input query files 
		else{
			char *inputfile = argv[4];
			char *outputfile = argv[5];
			sprintf(ipathandfile, "../%s/%s", pagedir, inputfile);
			sprintf(opathandfile, "../%s/%s", pagedir, outputfile);
			//ipathandfile = argv[5];
			//opathandfile = argv[6];
			printf("input query file at: %s\n", ipathandfile);
			printf("output file at: %s\n", opathandfile);
			inputquery = fopen(ipathandfile, "r");
			outputquery = fopen(opathandfile, "w");
			if (inputquery == NULL){
				printf("input file cannot be found");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// structures to handle strings 
	//char *strarr[MAXSTRINGS];
	char tempstr[MAXCHAR];
	
	char *word;
	char *str; 
	char newline;
 
	//checks
	bool loop = true;
	bool isalpha = false;
	bool islength = false;
	bool isUnvalidFirstWord = false;
	bool lastwordOp = false; 
	bool isUnvalidLastWord = false;
	bool adjacentOp = false; 
	//	bool firstword = true;
	

	// load index into hash

	hashtable_i* hash;
	hash = indexload(filename, pagedir);
	//	hash = indexload(filename, "pages-depth3");
	if (hash == NULL){
		exit(EXIT_FAILURE);
	}
	//happly(hash, print_hash);
	// open up queue of rankids
 
	queue_t* qrankid;
	
	// char newline;                                                                                                                              
	//  fscanf(inputquery, "%[^\n]%c", str, newline);                                                                                                
	//   printf("str: %s\n", str);       
	 //while user does not enter eof
			
	while (loop){

		// to indicate whether there was at least one document with rank above 0 
		bool hasresult = false;
		bool emptyString = false;
		bool isFirstWord = true;

		//open new queue every time
		qrankid = qopen();
		char *strarr[MAXSTRINGS];
		initstrarr(strarr);   
		int i = 0;	

		str = (char*) calloc(1, MAXSTRINGS * sizeof(char));
		//scanf(" %[^\n]%c", str, &newline); 
		
		//scan entire string from user if not quiet and read from input file if quiet
		if (!quiet){
			printf(">");
			scanf(" %[^\n]%c", str, &newline);
		}
		else{
			char newline[2];
		  fscanf(inputquery, " %[^\n]%c", str, newline);
			//	printf("str: %s\n", str);  
		}

		if (quiet){
			fprintf(outputquery, "str: %s\n", str);
		}
		else{
			printf("str: %s\n", str);
		}
		
		memset(tempstr, '\0', sizeof(MAXSTRINGS*sizeof(char)));
		
		//get first word of string 
		
		//printf("word within string: %s\n", word);
		word = strtok(str, " ");
		// if user types in empty string or no more input left in quiet mode 
		if (strcmp(str, "\0") == 0){
			//printstatement("empty string\n");                                                                                                          
       emptyString = true;                                                                                                                        
			if (quiet){
				//		printf("no more input in input file");
				exit(EXIT_SUCCESS);
			}
			else{
				printf("empty string\n");
				emptyString = true;
				
			}
																																																																									
		}
		
		// if string doesn't equal to NULL
		while (word != NULL && !emptyString){
			//resetstrarr(strarr);
			//freestrarr(strarr);
			//check conditions
			normalizeWord(word);
			int checkOperator = checkAndOr(word);
			
			//printf("checkoperator for word %s: %d\n", word, checkOperator);
			// if first word is and /or
			//break;
			if (isFirstWord && (checkAndOr(word) !=0)){
				printstatement("invalid query because first word is 'and' / 'or'\n");
				isUnvalidFirstWord = true;
				isFirstWord = false; 
				break;
			}
			if ((checkAndOr(word) != 0) && lastwordOp){
				printstatement("invalid query because adjacent 'and'/'or' combination\n");
				adjacentOp = true;
				break; 
			}
			// if the word is and / or
			if (checkAndOr(word) != 0){
				lastwordOp = true;
			}
			else {
				lastwordOp = false; 
			}
			// check if the word is eof
			if (checkEnd(word)==true)
				{
				printstatement("program terminated\n");
				loop=false;
				//	free(word);
				//printarrstr(strarr);
				break;
			}
			// check if word is actually a word 
			if (checkAlpha(word)){
				printstatement("invalid query because characters not in alphabet\n");
				//free(word);
				isalpha = true;
				break;
			}
			// check the length of the word (leave "or)
			if (checkLength(word) && (checkOperator != 1)){
				//				printstatement("invalid query because string less than 3 characters\n");
				//free(word);
				islength = true;
				
			}

			//exclude and,  non alphabets, short words 
			// if all conditions are good, put counts with each word 
			if ((checkOperator != -1) && !isalpha && !islength && !isUnvalidFirstWord && !adjacentOp){
				
				//store each word in strarr
				
				strarr[i] = word;
				/*
				strcpy(tempword, word);
				strcat(tempword, " ");
				
				// build string 
				strcat(tempstr, tempword);
				strcat(tempstr, ": ");
				
				// convert count to str type and concact to tempstr
				char countstr[2];
				sprintf(countstr, "%d", totalwordcount);
				strcat(tempstr, countstr);
				strcat(tempstr, " ");
				
				//reset count
				*/
				//update strarr position for next word
				i++;
			}
			// get next word
			word = strtok(NULL, " ");
			//printf("word within string: %s\n", word);

			// check if last word is "and"/"or"
			if (word == NULL && lastwordOp){
				printstatement("invalid query because 'and' /'or' at the end of query\n");
				isUnvalidLastWord = true;
				break;
			}
			
			//reset and update checks
			isalpha = false;
			islength = false;
			loop = true;
			isUnvalidFirstWord = false;
			isFirstWord = false;
			adjacentOp = false; 
			//			firstword= false;
			//updateLowestCount();
			//printf("printing words in str arr: query...\n");
			//			printarrstr(strarr);
			//free(str);
		}

		// go through words in str arr and update rank only if the string is valid 
		if (!isalpha  && loop && !emptyString && !isUnvalidFirstWord && !isUnvalidLastWord && !adjacentOp){
			bool wordAfterOr = true;
			hasresult = false;
			int id = 1;
			//			printarrstr(strarr);  
			// put in ranking
			// for all documents 
			while(id < 83){
				totalwordcount = 0;
				lowestcount = 0;
				int countTracker[MAXINTS];
				//initialize countTracker elements to -1;
				initcountTracker(countTracker);
				int countTrackPos = 0;
				//posiiton of strarr to get word
				int j = 0;
				// for each word in query
				while (strarr[j] != NULL){
					//		printf("word at pos %d: %s\n", j, strarr[j]);
					
					int checkOperator = checkAndOr(strarr[j]);
					// only update counts if word is not "or"
					if (checkOperator != 1){
						// find count for word at id i
						
						findWordandCount(hash, strarr[j], id);                                                                                                   						//printf("totalcount for word %s at id %d: %d\n", strarr[j], id, totalcount);                                                                                                                            
						// update count for the specific word if not first time through or 1st word after "or"
						// if first time through or first word after "or", lowest count = total count for the word                                      
						if (wordAfterOr || j==0){                                                                                                                                                 
							lowestcount = totalwordcount;
							wordAfterOr = false;
							//	printf("lowest count: %d\n", lowestcount);   
						}                                                                                                                                                                                 
						else{                                                                                                                                                                              
							updateLowestCount();                                                                                                                               //printf("lowest count: %d\n", lowestcount);                          
						}                                                                                                                                                                                 
						//	printf("lowest count after updating word %s: %d\n", strarr[j], lowestcount); 
					}
					// if the word is or, then put lowest count gathered so far into countTracker 
					else{
						// put lowest count into countTracker
						//reset lowest count
						countTracker[countTrackPos] = lowestcount;
						countTrackPos++;
						//			printcountTracker(countTracker);
						wordAfterOr = true;
						lowestcount = 0;
					}
					//move onto next word and reset total count					
					j++;
					totalwordcount= 0;
					// put in count of last word of query into countTracker
					if (strarr[j] == NULL){
						countTracker[countTrackPos] = lowestcount;
						lowestcount = 0;
					}
				}

				// get rank and put into rankid struct and into queue
				int addCountPos = 0;
				int rank = 0;
				// while it does not hit the last lowest count in the array (all elements are initialized to -1)
				while (countTracker[addCountPos] != -1){
					rank = rank + countTracker[addCountPos];
					addCountPos++;
					
				}
				//				printcountTracker(countTracker);
				//reset count                                                                                                                                                                      
				rankid_t* rankid = (rankid_t*)malloc(sizeof(rankid_t));
				
				// fill in rankid
				rankid->rank = rank;
				rankid->id = id;
				//printf("rankid rank: %d, id %d\n", rankid->rank, rankid->id);
				
				// fill in url for rankid
				char *url;
				url = getURL(id);
				rankid->url = url;
				//printf("rankurl from id %d: %s\n", id, rankid->url);
				
				//print rankid
				//printrankid((void*)rankid);
				if (rankid->rank !=0){
					qput(qrankid, rankid);
					hasresult = true;
				}
				else{
					delete_rankid((void*)rankid);
				}
				
				//reset lowest count 
				//				lowestcount = 0;
				
				// move i up 1 for string array position update  
				id++;
			}

			// if there were no ranks above 0 for query, print no results
			// if there was, after every query processed, print out queue of ranks
			if (hasresult == false){
				printstatement("no results found\n");
			}
			else{
				 printstatement("printing queue of rankids...\n");
				 if (!quiet){
					 qapply(qrankid, printrankid);
				 }
				 else{
					 qapply(qrankid, fprintrankid); 
				 }
			}
			//	printf("printing words query...\n");                                                                                                                                                             
			//      printarrstr(strarr); 
			free(str);

			// reset queue
			qapply(qrankid, delete_rankid);                                                                                                                                                                      
			qclose(qrankid); 
		}
		
		//this is when eof is called
		// free usused eof string
		// reset empty string boolean
		else{	
			free(str);
			qapply(qrankid, delete_rankid);
			qclose(qrankid); 
		}
		
	}
	//close files
	if (quiet){
		fclose(outputquery);
		fclose(inputquery);
	}
	// free hash and string array and queue of rankids
	//qapply(qrankid, delete_rankid);
	//	qclose(qrankid);
	//freestrarr(strarr);
	happly(hash, delete_wordqueue);    
	hclose(hash);
		
}
		
