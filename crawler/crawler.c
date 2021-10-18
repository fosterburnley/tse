#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/webpage.h"
#include "../utils/queue.h"
#include "../utils/hash.h"
//#include <queue.h>

void print_page(void * page_p){
  if (page_p == NULL){
    printf("page not found\n");
    return;
  }
  webpage_t* webpage_p = (webpage_t *) page_p;

	printf("%s\n", webpage_getURL(webpage_p));
}

bool searchfn1(void* element, const void* keyp){
		printf("in search function\n");
	//	webpage_t* webpage_p;
	//webpage_p = (webpage_t*) webpage;
	//char *key = (char *) keyp;
	if (element == keyp){
		printf("found\n");
		return true;
	}
	else{
		printf("not found\n");
		return false; 
	}
}


int main(void){
  
  // print "hello" used in step 1 of lab...
  //printf("hello\n");

  queue_t *page_queue;
  hashtable_t *page_hash;
  // 1. Create a new webpage at depth 0, with the seed URL: https://thayer.github.io/engs50/
  webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
    
  // 2. Fetch the webpage html to your local computer
  // 3. Check that the fetch succeeded and if not exit with EXIT_FAILURE
  if (webpage_fetch(page)){
    // 4. Scan the fetched html...
    int pos = 0;
    char * result;
    page_queue = qopen();  // queue to hold the newly created webpages for internal URLs
    page_hash = hopen(1000);
		
    while ((pos = webpage_getNextURL(page, pos, &result)) > 0){
      
      // ...and print all the URL's it contains, one per line, with an indicator to say it is internal or external
      if (IsInternalURL(result)){
				//	printf("Internal URL : %s\n", result);

				// Step 3 of Mod 4. Queue of Webpages. Need to make webapge types for the internal URLs and put into the queue
				webpage_t * new_page = webpage_new(result, 0, NULL);  // ?? might initialize with depth as "pos" ??

				// step 4 of Mod 4- hash of webpages 
				//check if webpage is in the hashtable
				// if not, then put it in hashtable and put it in queue
				// if yes, then do not put in hashtable or queue and move on
				webpage_t *found = (webpage_t*) hsearch(page_hash, searchfn1, webpage_getURL(new_page), (int) strlen(webpage_getURL(new_page)));

				printf("current webpage in question: %s\n", webpage_getURL(new_page));
				printf("URL length: %d\n", (int) strlen(webpage_getURL(new_page)));
				printf("webpage after finding in hash table: %s\n", webpage_getURL(found));
				if (found == NULL)
				 {
				hput(page_hash, (void*) new_page, webpage_getURL(new_page), (int) strlen(webpage_getURL(new_page)));
				qput(page_queue, (void*) new_page);
				}
				// STILL NEED TO FREE new_page **
      }
			else{
				printf("External URL : %s\n", result); 
      }
      free(result);
    }
    
  }
 else{
    free(page);
    exit(EXIT_FAILURE); 
  }
  
  // need to print the queue then close it
	printf("internal hash of webpages...\n");
	happly(page_hash, print_page);
  printf("internal queue of webpages...\n");
	qapply(page_queue, print_page);
	hclose(page_hash);
  qclose(page_queue);
  // 5. De-allocate the webpage and terminate it with EXIT_SUCCESS
  free(page);
  return EXIT_SUCCESS;
}
