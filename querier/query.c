#include <stdio.h>
#include <string.h>

int x = 1;
while(x = 0){
  int main(){
    char query[100];
    char * token, * delim;
        
    printf(">");
    scanf("%s", &query);

    if (strcmp(query, "^D") == 0){
      return 0;
    }
    
    token = strtok(query, " \t");
    while((delim = strtok(NULL, " \t") != NULL){
	query = strcat(query, delim);
	memset(delim, 0, strlen(delim));
      }
      
      for (int i = 0; i < strlen(query); i++){
	if (isalpha(query[i]) == 0){
	  printf("[invalid query]\n");
	  return 1;
	}
	query[i] = tolower(query[i]);
      }
      printf("%s", query);
      return 0;       
    
  }
