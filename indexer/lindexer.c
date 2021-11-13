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
#include <string.h>
#include "../utils/pageio.c"
#include "../utils/indexio.c"
//include "../utils/hash.c"
//include "../utils/queue.c"   
#include <stdbool.h>
#include <pthread.h>
#include "../utils/lhash.c"
#include "../utils/lqueue.c"


#define MAXWORD 100
#define HASHS 1000

//global count variable 
int totalcount = 0;
int totalhash = 0;
hashtable_t* sharedindex;
int sharedID;
pthread_mutex_t intm; 



//int frees = 0;
/*
 *the word count struct with a word and associated count 
*/
typedef struct countstruct{
  char* word;
  int count;

} count_i;

void unlockintMutex(){                                                                                                                                                                        
  printf("mutex %p unlocked\n", (void*)&intm);                                                                                                                                               
  pthread_mutex_unlock(&intm);                                                                                                                                                                  
}                                                                                                                                                                                              
                                                                                                                                                                                               
void lockintMutex(){                                                                                                                                                                          
  printf("mutex %p locked\n", (void*)&intm);                                                                                                                                                  
  pthread_mutex_lock(&intm);                                                                                                                                                                     
}    

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

void createThread(pthread_t* thread, void* (fn)(void* argp), void* argp){                                                                                                                     
  if (pthread_create(thread, NULL, fn, argp)!=0){                                                                                                                                             
    printf("program terminated: could not create thread");                                                                                                                                    
    exit(EXIT_FAILURE);                                                                                                                                                                       
  }                                                                                                                                                                                           
  else{                                                                                                                                                                                       
    //printf("within thread %d\n", threadid);                                                                                                                                                 
    //threadid++;                                                                                                                                                                            
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
	//	print_doccount((void*)(wqueue->queue));
 	qapply(wqueuehash->queue, print_doccount);
  //printf("count: %d\n", count_w->count);
}
*/
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
    
  }
  return 0;
}
void* indexPages(int thisID,  char* pagedir);

void* changeSharedID(void* argc){
	//	indexPages(argc);
  int thisID;
	
	//	lockintMutex();
	sharedID++;
	thisID = sharedID;
	printf("incrementing sharedID to %d\n", sharedID);
	// 	indexPages(argc);
	unlockintMutex();
	
	char* pagedir = (char*) argc;
	printf("running index over ID: %d\n", thisID);
	indexPages(thisID, pagedir);
	printf("finished running index over ID: %d\n", thisID);  

	return NULL;

}
 
void* indexPages(int thisID,  char* pagedir){
	//	char* pagedir = (char*) pagedir_v;
	// 	sleep(3);
	//lockMutex(&m);
	printf("scanning page: %d\n", sharedID); 
	//	hashtable_t* hword = (hashtable_t*) hword_v;
	 	// for each document
	webpage_t* webpage;  

	//	while(i !=83){
		//		webpage_t* webpage;
	//webpage = pageload(i, "pages-depth3");
	if ((webpage = pageload(thisID, pagedir))==NULL){
		printf("pageID %d in directory %s does not \n", thisID, pagedir);
		return NULL;
	}
	
		//	while(((webpage = pageload(i, pagedir))!=NULL)){
		//webpage = pageload(i, "pages-depth3");	
	printf("page loaded id: %d\n", thisID);
	//sleep(3);
	//		webpage = pageload(i, "pages-depth3");
	int pos = 0;
	char* word;
	while((pos = webpage_getNextWord(webpage, pos, &word))!=-1){
		
		// word to doccount 
		wqueue_i* wqueuehash;
		
		// doc id and count
		doccount_i* doccount;
		
		if (NormalizeWord(word) != 1){
			printf("current word for id %d: %s\n", thisID, word);
				//sleep(3);
			wqueue_i* foundwq = (wqueue_i*)(lhsearch(sharedindex, wqsearchfn, word, strlen(word)));
			//printf("test");
		  //if wordcount struct is found, add 1 to its count
		  //if not found, put wordcount element in hash and initialize 1 to its count
			
			if(foundwq!=NULL){	
				// search queue for doccount structs using id as key and count as value
				//					printf("id: %d, count: %d", foundwq->id, foundwq->count);
				//printf("finding doccount struct from queue in id : %d\n", i); 
				doccount_i* founddc = (doccount_i*)(lqsearch(foundwq->queue, dcsearchfn, &thisID));
				
				if (founddc !=NULL){
					printf("found word %s in id %d, adding 1 to count\n", word, thisID);
					//			sleep(3);
					founddc-> count = founddc->count +1;
					totalcount++;
					//free(word);
					//qapply(wqueuehash->queue, print_doccount);
					
				}
				else{
					//put in new doccount 
					doccount = (doccount_i*)malloc(sizeof(doccount_i));
					doccount ->id = thisID;  // hard coded to one for step 4
					doccount ->count = 1;
					totalcount++;
					printf("put word %s in queue for id %d\n", word, thisID);
					//	sleep(3);
					lqput(foundwq->queue, doccount);
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
				wqueuehash ->queue = lqopen();
				wqueuehash ->word = word;
				//printf("id : %d\n", i);
				// put in new doc count
				doccount = (doccount_i*)malloc(sizeof(doccount_i));
				doccount -> id = thisID;  // hard coded to one for step 4
				doccount ->count = 1;
			 	printf("doccount id: %d, doccount count: %d\n", doccount -> id, doccount ->count);
				//sleep(3);
				//put doccount in queue and put queue in hash 
				lqput(wqueuehash->queue, (void*) doccount);
				//qapply(wqueuehash->queue, print_doccount);
				lhput(sharedindex, (void*) wqueuehash, word, strlen(word));
				//totalhash++;
				totalcount++;
			}
		}
		else{
			free(word);		
		}
		
	}
	
	//		printf("sharedID: %d", sharedID);
	//	webpage_delete(webpage);
		

	//printf("totalhash: %d\n", totalhash);
	//happly(hword, print_hash);
	
	//printf("printing hash... \n");
 	//happly(sharedindex, print_hash);
	

	
	//	indexsave(hword, filename, pagedir);
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
	//happly(hword, delete_wordqueue);
	//happly(hload, delete_wordqueue);
	// delete hash 
	//hclose(hword);
	//hclose(hload);
	//	webpage_delete((void*)webpage);
	// free webpage
	// 	webpage=pageload(i-1, "pages-depth3");
	//pagesave(webpage, i+1, "pagestest");
	webpage_delete(webpage);
	
	//unlockMutex(&m);
	//	printf("total frees %d", frees);
	return (void*) sharedindex;
}
		

//int main(){
	int main(int argc, char *argv[]){
	
	if (argc < 4){
		printf("usage error: <pagedir> <indexnm> <number of threads>\n");
		exit(EXIT_FAILURE);
	}
	
  // initialize shared i = 0
	sharedID = 0;
	sharedindex = lhopen(HASHS); 
	
	int numberofThreads = atoi(argv[3]);
	char* pagedir = argv[1];
	char* filename = argv[2];

	pthread_t threads[numberofThreads];
	
	int currentThread = 1;
	// create threads
	while (currentThread <= numberofThreads){
		createThread(&threads[currentThread], changeSharedID, pagedir);
		currentThread++;
	}
	//update currentThread
	currentThread = 1; 
	// join threads
	while(currentThread <= numberofThreads){
		if(pthread_join(threads[currentThread], NULL)!=0){                                                                                                                                        
			exit(EXIT_FAILURE);                                                                                                                                                                     
  }                                                                                                                                                                                           
		else{                                                                                                                                                                                    
			printf("thread %d terminated\n", currentThread);                                                                                                                                      
		}   
		currentThread++;
	}
	/*	
		
	if(pthread_join(t1, NULL)!=0){                                                                                                                                                            
    exit(EXIT_FAILURE);                                                                                                                                                                       
  }                                                                                                                                                                                           
  else{                                                                                                                                                                                       
    printf("t1 terminated\n");                                                                                                                                                                
  }                                                                                                                                                                                           
                                                                                                                                                                                            
  if(pthread_join(t2, NULL)!=0){                                                                                                                                                              
    exit(EXIT_FAILURE);                                                                                                                                                                       
  }                                                                                                                                                                                           
  else{                                                                                                                                                                                       
    printf("t2 terminated\n");                                                                                                                                                                
  }                                                                                                                                                                                           
                                                                                                                                                                                             
  if(pthread_join(t3, NULL)!=0){                                                                                                                                                              
    exit(EXIT_FAILURE);                                                                                                                                                                       
  }                                                                                                                                                                                           
  else{                                                                                                                                                                                       
    printf("t3 terminated\n");                                                                                                                                                                
  }
	*/	

	//	lhapply(sharedindex, print_hash);
	pthread_mutex_destroy(&intm); 
	printf("total count %d\n", totalcount);	
	indexsave(sharedindex, filename, pagedir);
	lhapply(sharedindex, delete_wordqueue);   
	lhclose(sharedindex);

	
	exit(EXIT_SUCCESS);
}
