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
 *search for wordcount struct using word as key
*/ 
static bool wsearchfn(void* element, const void *key){
	count_i* count_w;
	count_w = (count_i*) element;
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
  
  /*
    else if (strlen(word) <3){
    return;
    }
    else{
    word[0] = tolower(word[0]);
    }
  */
  return 0;
}

 
int main(){
	webpage_t* webpage;
	char* word=NULL;
	hashtable_t* hword;
		
	int pos = 0;
	webpage = pageload(1, "pages-depth3");
	hword = hopen(MAXHASH);
 	
 	while((pos = webpage_getNextWord(webpage, pos, &word))!=-1){  // should this be > 0 ??
		count_i* count_w;
		count_w = (count_i*) (malloc(sizeof(count_i)));
	//webpage_getNextWord(webpage, pos, &word);
	//webpage_getNextWord(webpage, pos, &word);
		//		printf("current word: %s\n", word);

		if (NormalizeWord(word) != 1){
		  //printf("normalized word: %s\n", word);
		
		  count_w ->count = 1;
		  count_w ->word = word;

		  //search for word in hashtable
		//		bool result = wsearchfn(count_w->word, word);
	  //print_countword((void*)count_w);

		  count_i* found = (count_i*)(hsearch(hword, wsearchfn, word, strlen(word)));

		  // count_w = found;
		
		//if wordcount struct is found, add 1 to its count
		// if not found, put wordcount element in hash and initialize 1 to its count

		  if(found!=NULL){	
			//count_w -> word = found -> word;
			//printf("found word: %s, count: %d\n", found -> word, found->count);

		    found -> count = (found -> count) + 1;
		    printf("word : '%s' | count after updated : %d\n", found->word, found ->count);
		    free(count_w->word);
		    free(count_w);
			
		    //hput(hword, (void*) count_w, word, strlen(word));
		  }
		  else{
		    hput(hword, (void*) count_w, word, strlen(word));
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
