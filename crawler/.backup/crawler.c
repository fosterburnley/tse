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

static bool searchfn1(void* element, const void* keyp){
  webpage_t * webpage_p = (webpage_t *) element;
  char * key = (char *) keyp;
 
  if (strcmp(webpage_getURL(webpage_p),key) == 0){
    return true;
  }else{
    return false; 
  }
}

/*
 * save a webpage to a file in a certain directory 
 * id is the filename and dirname is the directory name 
 * file has URL, depth, length of HTML, HTML content
 */
static int32_t pagesave(webpage_t *pagep, int id, char *dirname){
	//write in a new file in directory name
	// name it id 1
	// put in the contents
	// close
	
 	//char *filename[10];
	//	itoa(id, filename, 10);
	char pathandfile[100];
	sprintf(pathandfile, "../%s/%d", dirname, id);
	//	printf("filename: %s", filename); 
  FILE *fp;
	fp = fopen(pathandfile, "w+"); //creates a file to write
	if(fp==NULL){
		return(1); //error opening file
	}
	printf("%s\n %d\n %d\n %s\n", webpage_getURL(pagep), webpage_getDepth(pagep), webpage_getHTMLlen(pagep), webpage_getHTML(pagep)); 
	fprintf(fp,"%s\n %d\n %d\n %s\n", webpage_getURL(pagep), webpage_getDepth(pagep), webpage_getHTMLlen(pagep), webpage_getHTML(pagep));
	//fprintf(fp,"%d\n", webpage_getDepth(pagep));
	//	fprintf(fp,"%d\n", webpage_getHTMLlen(pagep));
	//fprintf(fp,"%s\n", webpage_getHTML(pagep));
	fclose(fp);
	return(0);

}

int main(void){
  
  // print "hello" used in step 1 of lab...
  //printf("hello\n");
  int id = 1;
  queue_t *page_queue;
  hashtable_t *page_hash;
  // 1. Create a new webpage at depth 0, with the seed URL: https://thayer.github.io/engs50/
  webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	//char *html = webpage_getHTML(page);
	
  // 2. Fetch the webpage html to your local computer
  // 3. Check that the fetch succeeded and if not exit with EXIT_FAILURE
  if (webpage_fetch(page)){
    // 4. Scan the fetched html...
    int pos = 0;
    char * result;
    page_queue = qopen();  // queue to hold the newly created webpages for internal URLs
    page_hash = hopen(1000);

		//		id++;
    while ((pos = webpage_getNextURL(page, pos, &result)) > 0){
      
      // ...and print all the URL's it contains, one per line, with an indicator to say it is internal or external
      if (IsInternalURL(result)){
	// Step 3 of Mod 4. Queue of Webpages. Need to make webapge types for the internal URLs and put into the queue
				// must free new age 
				webpage_t * new_page = webpage_new(result, 0, NULL);
				if (webpage_fetch(new_page)){
				
				
	// Step 4 of Mod 4. Hash of webpages. 
	// check if webpage is in the hashtable
	// if not, then put it in hashtable and put it in queue
	// if yes, then do not put in hashtable or queue and move on
					webpage_t *found = (webpage_t*) hsearch(page_hash, searchfn1, webpage_getURL(new_page), (int) strlen(webpage_getURL(new_page)));

	//printf("current webpage in question: %s\n", webpage_getURL(new_page));
	//printf("URL length: %d\n", (int) strlen(webpage_getURL(new_page)));
					printf("webpage after finding in hash table: %s\n", webpage_getURL(found));

				     if (found == NULL){
						 pagesave(new_page, id, "pages");
						 id++;
	           hput(page_hash, (void*) new_page, webpage_getURL(new_page), (int) strlen(webpage_getURL(new_page)));
	           qput(page_queue, (void*) new_page);
			
	                }
				}
			}

			     	else {
						 printf("\nExternal URL : %s\n", result); 
					     }
      free(result);
			}    
		}
	  else{
    //free(page);
    exit(EXIT_FAILURE);
  }
  
  // need to print the hash table and queue...
  printf("\ninternal hash of webpages...\n");
  happly(page_hash, print_page);
  printf("\ninternal queue of webpages...\n");
  qapply(page_queue, print_page);
  // ... then close them.
  hclose(page_hash);
  qclose(page_queue);
	//	free(new_page);
  // 5. De-allocate the webpage and terminate it with EXIT_SUCCESS
  //free(page);  // removing this free decrease number of valgrind errors...
  return EXIT_SUCCESS;
}
