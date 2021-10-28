/* indexio.c --- save and load index to a file 
 * 
 * 
 * Author: Mikaela A. O'Brien
 * Created: Wed Oct 27 13:27:17 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <hash.h>
#include <queue.h>
//#include "../indexer/indexer.c"

#define MAXARRAY 1000
FILE *fp;                                                                                                                                

typedef struct doccount{
	int id;
	int count;
} doccount_i;

typedef struct wqueue{
	char* word;
	queue_t* queue;

} wqueue_i;


void getidCount(void* queue){
  int count; 
	int id;  
	doccount_i* idcountq = (doccount_i*) queue;
	count = idcountq->count;
	id = idcountq->id;
	fprintf(fp, "%d %d ", id, count);
}


void getWord(void *element){
  wqueue_i* wqhash = (wqueue_i*) element;
	char *word = wqhash->word;
	queue_t *queue = wqhash->queue;
	fprintf(fp, "%s ", word);
	qapply(queue, getidCount);
 	fprintf(fp, "\n");
 
}

/*indexsave() should save an index to a file named indexnm.
 * the index file shall contain one line for each word in the index
 * each line should have the following format:
 * <word> <docID1> <count1> <docID2> <count2>
 *
 * returns 0 for success, nonzero otherwise
 */

int32_t indexsave(hashtable_t *hashtable, char *filename, char *dirname){
  char pathandfile[100];
  sprintf(pathandfile, "../%s/%s", dirname, filename);
  printf("saving file in %s\n", pathandfile);
  fp = fopen(pathandfile, "w+");
  if (fp==NULL){
    printf("warning: file is NULL\n");
    return(1);
  }
  happly(hashtable, getWord);
  //	fprintf(fp, "\n");
  fclose(fp);
  return 0;
}

/* indexload should load the file <filename> in directory <dirname>
 *
 * returns non-NULL for success, NULL otherwise
 *
 */

hashtable_t *indexload(char *filename, char *dirname){
  char pathandfile[100];
  sprintf(pathandfile, "../%s/%s", dirname, filename);
  printf("loading file in %s\n", pathandfile);
  fp = fopen(pathandfile, "r");
  // ^ unsure if this is right ^ need to load/create and index for the file
  
}
