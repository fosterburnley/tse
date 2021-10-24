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

#define MAXWORD 100

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
		free(word);
		
	}
	
	//  free(word);
	
	webpage_delete(webpage);
	return 0;

}
