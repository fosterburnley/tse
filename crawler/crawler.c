#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <webpage.h>


int main(void){
  
  // print "hello" used in step 1 of lab...
  //printf("hello\n");

  // 1. Create a new webpage at depth 0, with the seed URL: https://thayer.github.io/engs50/
  webpage_t * page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
    
  // 2. Fetch the webpage html to your local computer
  // 3. Check that the fetch succeeded and if not exit with EXIT_FAILURE
  if (webpage_fetch(page)){

    // 4. Scan the fetched html... 
    //char * html = webpage_getHTML(page);  ... ?? Don't think we need this line ??
    int pos = 0;
    char * result;
    while ((pos = webpage_getNextURL(page, pos, &result)) > 0){
      
      // ...and print all the URL's it contains, one per line, with an indicator to say it is internal or external
      if (IsInternalURL(result)){
	printf("Internal URL : %s\n", result); 
      }else{
	printf("External URL : %s\n", result); 
      }
      free(result);
    }
    
  }else{
    exit(EXIT_FAILURE); 
  }
    
  // 5. De-allocate the webpage and terminate it with EXIT_SUCCESS
  free(page);
  return EXIT_SUCCESS;
}
