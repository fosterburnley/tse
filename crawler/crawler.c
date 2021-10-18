#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/webpage.h"
#include "../utils/queue.h"
//#include "../utils/hash.h"
//#include <queue.h>

void print_page(void * page_p){
  if (page_p == NULL){
    printf("page not found\n");
    return;
  }
  webpage_t* webpage_p = (webpage_t *) page_p;

	printf("%s\n", webpage_getURL(webpage_p));
}


int main(void){
  
  // print "hello" used in step 1 of lab...
  //printf("hello\n");

  queue_t *page_queue;
  
  // 1. Create a new webpage at depth 0, with the seed URL: https://thayer.github.io/engs50/
  webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
    
  // 2. Fetch the webpage html to your local computer
  // 3. Check that the fetch succeeded and if not exit with EXIT_FAILURE
  if (webpage_fetch(page)){

    // 4. Scan the fetched html...
    int pos = 0;
    char * result;
    page_queue = qopen();  // queue to hold the newly created webpages for internal URLs
    
    while ((pos = webpage_getNextURL(page, pos, &result)) > 0){
      
      // ...and print all the URL's it contains, one per line, with an indicator to say it is internal or external
      if (IsInternalURL(result)){
				printf("Internal URL : %s\n", result);

				// Step 3 of Mod 4. Queue of Webpages. Need to make webapge types for the internal URLs and put into the queue
				webpage_t * new_page = webpage_new(result, 0, NULL);  // ?? might initialize with depth as "pos" ??
				qput(page_queue, (void*) new_page);
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
	printf("internal queue of webpages...\n");
	qapply(page_queue, print_page);
  qclose(page_queue);  
  // 5. De-allocate the webpage and terminate it with EXIT_SUCCESS
  free(page);
  return EXIT_SUCCESS;
}
