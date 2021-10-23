/* pagetest.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Sat Oct 23 14:02:04 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "../utils/pageio.c"

int main(){
 	//webpage_t* webpage =  pageload(1, "pages-depth3");
	
	 webpage_t* webpage = pageload(1, "pages-depth3");
	 pagesave(webpage, 1, "pagestest");
	 webpage_t* newpage = pageload(1, "pagestest");
	 pagesave(webpage, 2, "pagestest"); 
	//	printf("url %s\n", webpage_getURL(webpage));
	//printf("html  %s\n", webpage_getHTML(webpage));                                                                                                              

	webpage_delete(webpage);
	webpage_delete(newpage);
	//webpage_delete(new_page);
	return 0;
}
