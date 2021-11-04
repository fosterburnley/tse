/* indextest.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Wed Oct 27 15:26:02 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdlib.h>
#include<stdio.h> 
#include "../utils/indexio.c"
#include "../utils/hash.c"


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


int main(){
	
	hashtable_i* hload;
	//	hload = hopen(1000);
	hload = indexload("indexnm1", "pages-depth3");
	printf("printing hash after loaded by indexnm..\n");	
	happly(hload, print_hash);
	/*
	indexsave(hload, "indexnm1", "indexes");
	happly(hload, delete_wordqueue);
	//printf("saving indexnm..\n"); 
	*/
	hclose(hload);

	
}
