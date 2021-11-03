/* indexer.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Sun Oct 24 13:34:00 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */


#include <stdlib.h>
#include <stdio.h>
#include "../utils/pageio.c"
#include "../utils/indexio.c"
#include "../utils/hash.c"
#include "../utils/queue.c"   
#include <stdbool.h>
#define MAXWORD 100
#define MAXHASH 1000

//global count variable 
int totalcount = 0;
int totalhash = 0;
//int frees = 0;
/*
 *the word count struct with a word and associated count 
*/
typedef struct countstruct{
  char* word;
  int count;

} count_i;


/*
 * struct that holds a word and a queue of doccount structs
 */
/*
typedef struct wqueue{
  char * word;

	//queue of doccounts 
  queue_t * queue; 

} wqueue_i;

/*
 * struct that contains document ID and the occurences of a given word in that document
 
typedef struct doccount{
  int id;
  int count;
  
} doccount_i;
*/

/*
 *search for wordqueue struct using word as key
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
 * element = doccount struct
 * key is int that you are searching for in doccount struct
 */
static bool dcsearchfn(void *element, const void *key){
	int* pid = (int*)key;
	int currentid = *pid;
	doccount_i* doccount = (doccount_i*) element;
	//int* count = (int*)element;
	//printf("doc count id: %d", doccount->id);
	//	if (doccount->id == NULL){
	//	return false; 
	//}
	if (doccount -> id == currentid){
		return true;
	}
	else{
		return false;
	}
	
}

void print_doccount(void *doccount_v){
	//printf("hello");
	doccount_i* doccount= (doccount_i*)(doccount_v);
	printf("id: %d, count: %d\n\n", doccount-> id, doccount->count); 

}
/*
 * print the word from wordcount struct 
*/ 
void print_hash(void *wqueue_v){
	
  wqueue_i* wqueuehash = (wqueue_i*) wqueue_v;
	
  printf("word in hash: %s\n", wqueuehash->word);
	//	print_doccount((void*)(wqueue->queue));
 	qapply(wqueuehash->queue, print_doccount);
  //printf("count: %d\n", count_w->count);
}

/*
 * sum the counts in hash table
*/ 
void sum_count(void *counttype){
  count_i* count_w;
  count_w = (count_i*) counttype;
  totalcount = totalcount+ count_w->count;
}

/*
 * int / int (only free struct) 
 */
void delete_keycount(void* doccount_v){
	doccount_i* doccount = (doccount_i*) doccount_v;
	
	free(doccount);
}
/*
* free all the blocks in a word count struct
* free word 
* close queue
* free each doccount struct
*/ 
void delete_wordqueue(void *wqueue_v){
  wqueue_i* wqueuehash = (wqueue_i*) wqueue_v;
  free(wqueuehash->word);

  qapply(wqueuehash->queue, delete_keycount);
	qclose(wqueuehash->queue);
	free(wqueuehash);
}

/*
 *lowercase word that is greater than 3 letters and only contains letters
*/ 
int NormalizeWord(char* word){
	//check for length
  if (strlen(word) < 3){
    return 1; 
  }
  for (int i = 0; i < strlen(word); i++){ 
    if (isalpha(word[i]) == 0){
      return 1;
    }
    word[i] = tolower(word[i]);
		//		printf("currentword: %s", word);
    return 0;     
  }
  return 0;
}

 
int main(int argc, char *argv[]){
	if (argc < 3){
		printf("usage error: <pagedir> <indexnm>");
		exit(EXIT_FAILURE);
	}

	hashtable_t* hword;
	
	hword = hopen(MAXHASH);
 	// for each document
	int i = 1;
	char *filename = argv[2];
	webpage_t* webpage;  
	char *pagedir = argv[1];
	//	while(i !=83){
		//		webpage_t* webpage;
	//webpage = pageload(i, "pages-depth3");
	while(((webpage = pageload(i, pagedir))!=NULL && i <3)){  
		//	while(((webpage = pageload(i, pagedir))!=NULL)){
		//webpage = pageload(i, "pages-depth3");	
		printf("page loaded id: %d\n", i);
		//		webpage = pageload(i, "pages-depth3");
		int pos = 0;
		char* word;
		while((pos = webpage_getNextWord(webpage, pos, &word))!=-1){

		// word to doccount 
			wqueue_i* wqueuehash;
			
		// doc id and count
			doccount_i* doccount;
			
			if (NormalizeWord(word) != 1){
				//	printf("current word: %s\n", word);	
				wqueue_i* foundwq = (wqueue_i*)(hsearch(hword, wqsearchfn, word, strlen(word)));

		  //if wordcount struct is found, add 1 to its count
		  //if not found, put wordcount element in hash and initialize 1 to its count

				if(foundwq!=NULL){	
		   					// search queue for doccount structs using id as key and count as value
					//					printf("id: %d, count: %d", foundwq->id, foundwq->count);
					//printf("finding doccount struct from queue in id : %d\n", i); 
					doccount_i* founddc = (doccount_i*)(qsearch(foundwq->queue, dcsearchfn, &i));

					if (founddc !=NULL){
						//printf("found word- adding 1 to count\n");
						founddc-> count = founddc->count +1;
						totalcount++;
						//free(word);
						//qapply(wqueuehash->queue, print_doccount);
						
					}
					else{
						//put in new doccount 
						doccount = (doccount_i*)malloc(sizeof(doccount_i));
						doccount ->id = i;  // hard coded to one for step 4
						doccount ->count = 1;
						totalcount++;
						//	 			printf("id to be put in queue : %d\n", i); 
						qput(foundwq->queue, doccount);
						//qapply(wqueuehash->queue, print_doccount);
					}
					free(word);
					//delete_wordqueue(wqueuehash);
					//free(wqueuehash);
				 //				 qclose(wqueuehash->queue);
				 //free(wqueuehash->word);
				 
					
								
		    //hput(hword, (void*) count_w, word, strlen(word));
				}
				else{
					// fill in wordqueue struct and put in hash
					wqueuehash = (wqueue_i*) (malloc(sizeof(wqueue_i)));
					wqueuehash ->queue = qopen();
					wqueuehash ->word = word;
					//printf("id : %d\n", i);
					// put in new doc count
					doccount = (doccount_i*)malloc(sizeof(doccount_i));
					doccount -> id = i;  // hard coded to one for step 4
					doccount ->count = 1;
					//					printf("doccount id: %d, doccount count: %d\n", doccount -> id, doccount ->count);
					//put doccount in queue and put queue in hash 
					qput(wqueuehash->queue, (void*) doccount);
	 				//qapply(wqueuehash->queue, print_doccount);
					hput(hword, (void*) wqueuehash, word, strlen(word));
					//totalhash++;
					totalcount++;
				}
			}
			else{
				free(word);		
			}
	
		}
		i++;
		webpage_delete(webpage);
		
		//free(word);
		/*
		if ((webpage=pageload(i+1, "pages-depth3"))!=NULL){
				i++;
			}
		else{
			//webpage= pageload(i, "pages-depth3");
			//webpage_delete((void*)webpage);
			printf("last id %d\n", i);
			break;
		}
		*/
		
	}
	
		 

		//printf("totalhash: %d\n", totalhash);
		//happly(hword, print_hash);

	printf("printing hash... \n");
 	happly(hword, print_hash);

	printf("total count %d\n", totalcount);

	indexsave(hword, filename, pagedir);
	//printf("loading indexnm...");
	
		//	hashtable_i* hload;
	//hload = hopen(1000000);
  //hload = indexload("indexnm", "indexes");
	//printf("printing hash after loaded by indexnm..");
	//happly(hload, print_hash);
	//indexsave(hload, "indexnm2", "indexes"); 	
	//sum counts
	//happly(hword, sum_count);
	//printf("TOTALCOUNT: %d\n", totalcount);

// free remaining bloscks
	// free word, close queue, free doccounts within the queue
	// do that for all wordqueue structs in hash 
	happly(hword, delete_wordqueue);
	//happly(hload, delete_wordqueue);
	// delete hash 
	hclose(hword);
	//hclose(hload);
	//	webpage_delete((void*)webpage);
	// free webpage
	// 	webpage=pageload(i-1, "pages-depth3");
	//pagesave(webpage, i+1, "pagestest");
	webpage_delete(webpage);


	//	printf("total frees %d", frees);
	return 0;
}
