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
#include <stdbool.h>
#define MAXWORD 100

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
	//	word = (char*) calloc(1, MAXWORD*sizeof(char));
	int pos = 0;
	webpage = pageload(1, "pages-depth3");
	
 	while((pos = webpage_getNextWord(webpage, pos, &word))!=-1){
	//webpage_getNextWord(webpage, pos, &word);
	//webpage_getNextWord(webpage, pos, &word);
		printf("current word: %s\n", word);
		NormalizeWord(word);
		printf("normalized word: %s\n", word); 
		free(word);
		
	}
	
	//  free(word);
	
	webpage_delete(webpage);
	return 0;
}
