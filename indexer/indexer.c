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

int totalcount = 0;

typedef struct countstruct{
	char* word;
	int count;

} count_i;

static bool wsearchfn(void* element, const void *key){
	count_i* count_w;
	count_w = (count_i*) element;
	char *word;
	word = (char*) key;
	if (count_w->word == NULL){
		return false;
	}
	
	if (strcmp(count_w->word, word)==0){
		printf("found true");
		return true;
	}
	else{
		printf("found false");
		return false;
	}

}

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
void sum_count(void *counttype){
	count_i* count_w;
	count_w = (count_i*) counttype;
	totalcount = totalcount+ count_w->count;
}

void delete_countstruct(void *count_v){
	count_i* count_w = (count_i*) count_v;
	free(count_w->word);
	
	free(count_w);
}


void NormalizeWord(char* word){
	//check for length
	if (!isalpha(word[0])){
			return;
	}
	else if (strlen(word) <3){
		return;
	}
	else{
		word[0] = tolower(word[0]);
	}
		
}


int main(){
	webpage_t* webpage;
	char* word=NULL;
	hashtable_t* hword;
	
	
	
	int pos = 0;
	webpage = pageload(1, "pages-depth3");
	hword = hopen(MAXHASH);

	
 	while((pos = webpage_getNextWord(webpage, pos, &word))!=-1){
		count_i* count_w;
		count_w = (count_i*) (malloc(sizeof(count_i)));
	//webpage_getNextWord(webpage, pos, &word);
	//webpage_getNextWord(webpage, pos, &word);
		//		printf("current word: %s\n", word);
		NormalizeWord(word);
		printf("normalized word: %s\n", word);
		
		count_w ->count = 1;
		count_w ->word = word;
		//search for word in hashtable
		//		bool result = wsearchfn(count_w->word, word);
	  //print_countword((void*)count_w);

		count_i* found = (count_i*)(hsearch(hword, wsearchfn, word, strlen(word)));
		//		count_w = found;
		
		//if found, add 1 to count
		// if not found, put struct in hash and add 1 to count
		if(found!=NULL){	
			//count_w -> word = found -> word;
			//printf("found word: %s, count: %d\n", found -> word, found->count);
			found -> count = (found -> count) + 1;
			printf("word: %s  and count after updated %d\n", found->word, found ->count);
			free(count_w->word);
			free(count_w);
			
			//hput(hword, (void*) count_w, word, strlen(word));
		}
		else{
			hput(hword, (void*) count_w, word, strlen(word));
		}


		//	happly(hword, print_countword);
	}
	printf("printing hash..");
	happly(hword, print_countword);


	//sum counts



	happly(hword, sum_count);
printf("totalcount: %d", totalcount);
	happly(hword, delete_countstruct);
	hclose(hword);
	webpage_delete(webpage);
	return 0;
}
