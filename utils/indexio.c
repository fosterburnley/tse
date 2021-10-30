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
#define MAXWORD 100
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
		printf("warning: cannot find directory\n");
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

hashtable_t* indexload(char *filename, char *dirname){
	char pathandfile[MAXARRAY]="";
 	char* word;
	char newline;
	doccount_i* doccount;
	wqueue_i* wqhash;
	int id=0;
	int count=0; 
	hashtable_t* hword;
	queue_t* hqueue;
	bool wempty=false;
	bool dempty=false; 
	char tempword[MAXARRAY]="";
	
	sprintf(pathandfile, "../indexes/indexnm");
	FILE *fp;
	fp = fopen(pathandfile, "r");
	printf("file at %s\n", pathandfile);
	
	if (fp == NULL){
		printf("warning: can't find file for index");
		return NULL;
	}
	//word = (char*) malloc(sizeof(char)); 
	//fscanf(fp, "%s", word);
 	//strcpy(lword, wordArray);
	//	printf("word:%s\n", word);
		
	hword = hopen(MAXARRAY);
	while(!wempty){
   	//while in the same word
		// put word in wqhash
		//strcpy(word, wordArray);

	  memset(tempword, '0', sizeof(tempword)); 
		fscanf(fp, "%s", tempword);
		printf("tempword: %s\n", tempword);
		if(tempword[0]=='0'){
			//printf("end of file: can't find word\n");
			wempty=true;
			break;
		}
		word = (char*)malloc(MAXWORD*(sizeof(char)));
		strcpy(word, tempword);
		printf("word: %s\n", word);	

		
	  wqhash= (wqueue_i*)malloc(sizeof(wqueue_i));
		//		word = (char*)sizeof(char);
		wqhash->word = word;
			hqueue = qopen();
		while(!dempty){
		//fscanf(fpload, "%d %d", id, count);
		
			doccount = (doccount_i*)malloc(sizeof(doccount_i));
			fscanf(fp, "%d", &id);
			//if (id==-1 && count ==-1){
			//dempty=true;
			//break;
			//	}  
			fscanf(fp, "%d[^\n]", &count);
			if (id == -1 && count == -1){
				dempty=true;
				free(doccount);
				break;
			}
			//newline = fgetc(fp);
			//printf("%c fscanf result: %c", newline);
			//			printf("id: %d, count: %d\n", id, count);
			doccount -> id = id;
			doccount->count = count;

			// put doccount in queue
			qput(hqueue, doccount);

			// put queue in wqhash
			wqhash->queue = hqueue;

			id = -1;
			count = -1;
			//if reach end of line
			//if (id == 0 && count == 0){
				//dempty=true;
			//fgetc(fp);
			//fgetc(fp);
			//}
		}
		// put wqhash in hash with word as key
		hput(hword, wqhash, word, strlen(word));
		dempty=false;
		//		word= "";
		//if (fgetc(fp) == EOF){
			//wempty=true;
			//}
	}
	
	fclose(fp);
	//	hclose(hword);
	return hword;
}

