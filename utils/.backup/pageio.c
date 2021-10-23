/* pageio.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Sat Oct 23 13:11:32 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <webpage.c>
#include <string.h>
#include <stdbool.h>
#define MAXARRAY 10000
/*
 * saves a page to pages directory 
 */
int32_t pagesave(webpage_t *pagep, int id, char *dirname){                                                      
  char pathandfile[100];                                                                                               
  sprintf(pathandfile, "../pages/%s/%d", dirname, id);                                                                       
  FILE *fp;                                                                                                            
  fp = fopen(pathandfile, "w+");                                                                                       
  if (fp==NULL){                                                                                                       
    return (1);                                                                                                        
  }

	// webpage_getHTMLlen adjusted to account for extra new line added in load page

	int htmllenAdj = (webpage_getHTMLlen(pagep))-1;
  printf("pagesaved at %s: %s with current depth: %d\n", pathandfile, webpage_getURL(pagep), webpage_getDepth(pagep)); 
  fprintf(fp, "%s\n%d\n%d%s\n", webpage_getURL(pagep), webpage_getDepth(pagep), htmllenAdj, webpage_getHTML(pagep));                                                                                                    
  fclose(fp);                                                                                                          
  return 0;                                                                                                            
}    

/*
bool isEmpty(char *string){

	for (int i = 0; i < sizeof(string); i++){
		if (string[i] != '0'){
		  return false;
	}
	return true;
	

}
*/
webpage_t *pageload(int id, char *dirnm){
	
	char url[MAXARRAY];
	char html[MAXARRAY];
	char temphtml[MAXARRAY];
	int htmllen;
	int depth;
	webpage_t* webpage; 
	bool isempty=false; 
	
	
	char pathandfile[MAXARRAY];
	// get file and store in variable
	sprintf(pathandfile, "../pages/%s/%d", dirnm, id);
	
	FILE *fp = fopen(pathandfile, "r");
	// scan document
	if (fp == NULL){
		printf("warning: no such file");
		return NULL;
	}

	
	// scan in variables 
	fscanf(fp, "%s %d %d", url, &depth, &htmllen);
	while (fscanf(fp, "%[^\n]", temphtml)!=EOF){
		fgetc(fp);
		printf("temphtml: %s\n", temphtml);

		//appends new line to html array
		//strcat(html, temphtml);
		//reset temphtml and replaces new lines with spaces
		for (int i = 0; i < sizeof(temphtml); i++){
			if (temphtml[i] !='0'){
				isempty = false;
				break;
			}
			isempty=true;
		}
		
	  if (isempty){
			printf("empty temphtml");
			strcat(html, "\n");
		}
		else{
			printf("not empty temphtml");
			strcat(html, temphtml);
			strcat(html, "\n");
		}
		memset(temphtml, '0', sizeof(temphtml));
	}

					 
				 
	
	//	printf("url: %s\n", url);
	//printf("depth: %d\n", depth);
	//printf("htmllen: %d\n", htmllen);
	//printf("html: %s\n", html);                                                                                         
	// get url

	//get html
	//	webpage_t* webpage = webpage_new(	
	fclose(fp);

	//depth - 1 to compensate for extra line added at the end of the html load process
	webpage = webpage_new(url, depth, html);
	return webpage;

}				 



	


