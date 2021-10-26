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
#include "../utils/hash.c"
#include "../utils/queue.c"   
#include <stdbool.h>
#define MAXWORD 100
#define MAXHASH 1000

//global count variable 
int totalcount = 0;

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
typedef struct wqueue{
  char * word;
  queue_t * queue; 

} wqueue_i;

/*
 * struct that contains document ID and the occurences of a given word in that document
 */
typedef struct doccount{
  int id;
  int count;
  
} doccount_i;

/*
 *search for wordcount struct using word as key
*/ 
static bool wsearchfn(void* element, const void *key){
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
 * print the word from wordcount struct 
*/ 
void print_countword(void *count_v){
	
  count_i* count_w;
  count_w = (count_i*) count_v;
  if(count_w == NULL){
    //		printf("word not found\n");
    return;
  }
  printf("word in hash: %s\n", count_w->word);
  printf("count: %d\n", count_w->count);
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
* free all the blocks in a word count struct
*/ 
void delete_countstruct(void *count_v){
  count_i* count_w = (count_i*) count_v;
  free(count_w->word);
  
  free(count_w);
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
    return 0;     
  }  
  return 0;
}

 
int main(){
	webpage_t* webpage;
	char* word=NULL;
	hashtable_t* hword;
		
	int pos = 0;
	wcdbpage = pageload(1, "pages-depth3");
	hword = hopen(MAXHASH);
 	
 	while((pos = webpage_getNextWord(webpage, pos, &word))!=-1){ 
		wqueue_i* wqueue;
		doccount_i* doccount;
		wqueue = (wqueue_i*) (malloc(sizeof(wqueue_i)));
	
		if (NormalizeWord(word) != 1){

		  doccount ->id = 1;  // hard coded to one for step 4
		  doccount ->count = 1;
		  
		  wqueue ->queue = qopen();
		  qput(queue, (void*) doccount);
		  wqueue ->word = word;
		  
		  wqueue_i* found = (wqueue_i*)(hsearch(hword, wsearchfn, word, strlen(word)));
		  		
		  //if wordcount struct is found, add 1 to its count
		  //if not found, put wordcount element in hash and initialize 1 to its count

		  if(found!=NULL){	
		    // found is a representation of the queue holding doccount structs
		    // will need to get into the queue, and then search again for the document we are working with
		    // (right now we have document id hardcoded as 1, but this will probably need to change and ride...
		    // ... off an outer while loop that scans over all the documents
		    // once we have the doccount struct for the given word in the given document,
		    // need to increment count by 1
		    // ~ below needs to be updated using above logic (12pm 10/26/21) ~ 

		    found -> count = (found -> count) + 1;
		    printf("word : '%s' | count after updated : %d\n", found->word, found ->count);
		    free(count_w->word);
		    free(count_w);
			
		    //hput(hword, (void*) count_w, word, strlen(word));
		  }
		  else{
		    hput(hword, (void*) wqueue, word, strlen(word));
		  }
		}
	}
	printf("printing hash... \n");
	happly(hword, print_countword);


	//sum counts
	happly(hword, sum_count);
	printf("TOTALCOUNT: %d\n", totalcount);

// free remaining bloscks
	happly(hword, delete_countstruct);
	hclose(hword);
	webpage_delete(webpage);
	return 0;
}
