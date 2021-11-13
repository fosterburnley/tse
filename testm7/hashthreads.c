/* hashthreads.c ---                                                                                                                                         
 *                                                                                                                                                       
 *                                                                                                                                                       
 * Author: Zachary J. Wong                                                                                                                               
 * Created: Sat Nov  6 15:06:39 2021 (-0400)                                                                                                             
 * Version:                                                                                                                                              
 *                                                                                                                                                       
 * Description: multi-thread processing of a shared hash table using lqhash and hash.h                                                                                                                                          
 */  


#include <queue.h>
#include <hash.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "../utils/lhash.c"
#include <pthread.h>
#include <unistd.h>

#define MAXREG 10
#define HASHSIZE 1000

int numberofPuts = 0;
//#define MAXCHAR 10
int count = 0;
hashtable_t* sharedhash;

int zach = 0;
int foster = 0;
int mikaela = 0;

typedef struct person {                                                       
  char name[MAXREG];                                                   
   int age;                                                               
   int gradYear;
	int fav;
	//int age;
} person_t;

typedef struct element_htest {
	void *data;
	char key[MAXCHAR];
} element_htest;

void delete_person(void* person_v){                                                                                                                      
                                                                                                                                                         
  person_t* person = (person_t*) person_v;                                                                                                               
	// printf("freeing person: %s\n", person->name);                                                                                                       
  free(person);                                                                                                                                          
                                                                                                                                                         
}                                                                                                                                                         
     
person_t* make_person(char* name, int age, int gradYear, int fav){
	person_t* pp=NULL;
	pp=(person_t*)malloc(sizeof(person_t));
	// fail case
	if (pp == NULL){
		//print to stderr file
		fprintf(stderr, "[Error: malloc failed allocating person]\n");
		return NULL;
	}
	//fill in data
	strcpy(pp->name, name);
	//strcpy(pp->gradYear, gradYear);
	//	strcpy(pp->fav, fav); 
	pp->age = age;
	pp->gradYear = gradYear;
	pp -> fav = fav;
	//	printf("testing print name %s, gradYear %d, fav %d, age %d \n\n", pp->name, pp->gradYear, pp->fav, pp->age);
	//printf("make person name %s, age %d \n\n", pp->name, pp->age); 
	return pp;

}

/*
 * print the data of the person type
 * return if successful
 */

void print_person(void *vp){
	//printf("%p\n\n", vp);
			if (vp == NULL){
		printf("person not found! \n\n");
		return;
		
		}
	//	printf("made it past if, person exists\n\n");
	person_t* pp1;
	//person_t* pp2;
	pp1=(person_t*) vp;
	//pp2 = pp1 -> data;
	//	strcpy(pp1->name, "zach");
	//pp1 -> age = 3;
	//pp1 -> gradYear = 30;
	
	//printf("printing person...\n\n");
	//  printf("name: %s, gradYear = %d, fav = %d, age = %d \n\n", pp->name, pp->gradYear, pp->fav, pp->age);
	printf(" name %s, age %d, gradYear %d, fav %d \n\n",  pp1->name, pp1->age, pp1->gradYear, pp1 ->fav);
	
	//printf("test");
}

static bool searchfn(void* elementp, const void* keyp){
	person_t* elementperson;
	elementperson = (person_t*)elementp;
	char *key = (char *)keyp;
	
	if((strcmp(elementperson->name, key))==0){
	  //		printf("in if statement\n\n");
		return(true);
	}
	else return(false);
}

void createThread(pthread_t* thread, void* (fn)(void* argp), void* argp){
  if (pthread_create(thread, NULL, fn, argp)!=0){
    printf("program terminated: could not create thread");
    exit(EXIT_FAILURE);
  }
  else{
    //printf("within thread %d\n", threadid);
    //threadid++;
  }
}

void* tput(void* argp){
  char* personname = (char*) argp;

  int i = 0;

		person_t* person;
    //sleep(4);
    person = make_person(personname, 20, 2023, 500);
		
		//    printf("int 1 tput: %d\n", i);
    printf("putting %s in shared hash... \n", personname);
    lhput(sharedhash, (void*) person, personname, strlen(personname)); 
		//    printf("printing hash...\n");
    //lhapply(sharedhash, print_person);

		numberofPuts++;


  return NULL;
}



void* tsearch(void* argp){
  char* personname = (char*) argp;
  int i = 1;

    //sleep(2);
    //printf("in ti tsearch: %d\n", i);
    person_t* found;
    printf("searching for %s from sharedhash...\n", personname);

    found = (person_t*) lhsearch(sharedhash, searchfn, personname, strlen(personname));
    if (found == NULL){
      printf("could not find person named %s in shared hash\n", personname);
    }
    else{
      printf("found person in shared hash: \n");
      print_person((void*)found);
    }


		

  return NULL;
}

void* tremove(void* argp){
  char* personname = (char*) argp;
  printf("removing person named %s from the shared hash..\n", personname);
  person_t* removed;
	int i = 0;

		removed = (person_t*) lhremove(sharedhash, searchfn, personname, strlen(personname));
		if (removed == NULL){
			printf("could not find and remove person named %s in shared hash\n", personname);
			return NULL;
		}
		else{
			printf("removed person in shared hash: \n");
			print_person((void*) removed);
			if (strcmp(removed->name, "zach") == 0){                                                                                                           
				zach++;
				printf("zach count: %d\n", zach);
			}                                                                                                                                                  
			else if (strcmp(removed->name, "foster") == 0){                                                                                                    
				foster++;
				printf("foster count: %d\n", foster);  
			}                                                                                                                                                  
			else if (strcmp(removed->name, "mikaela") ==0){                                                                                                    
				mikaela++;
				printf("mikaela count: %d\n", mikaela);  
			}               
			//printf("printing resulting shared hash after removal of %s: \n", personname);
			//			lhapply(sharedhash, print_person);
			delete_person(removed);
		}
		//delete_person(removed); 


  return NULL;
}

int main(){
  pthread_t t1 = 0;
  pthread_t t2 = 0;
  pthread_t t3 = 0;
  pthread_t t4 = 0;
  pthread_t t5 = 0;

  sharedhash = lhopen(HASHSIZE);
	int currentThread = 1;                                                                                                                                                             
  int numberofThreads = 82;                                                                                                                                                          
  pthread_t threads[numberofThreads];                                                                                                                                               
                                                                                                                                                                                    
  while (currentThread <= numberofThreads){                                                                                                                                          
    createThread(&threads[currentThread], tput, "zach");                                                                                                                             
    currentThread++;

		createThread(&threads[currentThread], tput, "foster");
		currentThread++; 
  }                                                                                                                                                                                 
                                                                                                                                                                                    
  //update currentThread                                                                                                                                                            
                                                                                                                                                                                    
  currentThread = 1;                                                                                                                                                               
                                                                                                                                                                                     
  // join threads                                                                                                                                                                   
                                                                                                                                                                                     
  while(currentThread <= numberofThreads){                                                                                                                                          
                                                                                                                                                                                   
    if(pthread_join(threads[currentThread], NULL)!=0){                                                                                                                                                                                                                                                                                                                 
      exit(EXIT_FAILURE);                                                                                                                                                           
                                                                                                                                                                                    
  }                                                                                                                                                                                 
                                                                                                                                                                                    
    else{                                                                                                                                                                           
                                                                                                                                                                                    
      printf("thread %d terminated\n", currentThread);                                                                                                                              
                                                                                                                                                                                    
    }                                                                                                                                                                               
    currentThread++;
  }

	currentThread = 1;
	while(currentThread <= numberofThreads){
		createThread(&threads[currentThread], tremove, "zach");
		currentThread++;
		createThread(&threads[currentThread], tremove, "foster");
		currentThread++;

	}

	currentThread =1;
	while(currentThread <= numberofThreads){
		if(pthread_join(threads[currentThread], NULL)!=0){
			exit(EXIT_FAILURE);
		}
		else{
			printf("thread %d terminated\n", currentThread);
		}
		currentThread++;
	}
	
	/*
  createThread(&t1, tput, "zach");
  createThread(&t2, tput, "foster");
  createThread(&t3, tput, "mikaela");
 
	
  if(pthread_join(t1, NULL)!=0){
		exit(EXIT_FAILURE);
	}
	else{
		printf("t1 terminated\n");
	}
	
	if(pthread_join(t2, NULL)!=0){
		exit(EXIT_FAILURE);
	}
	else{
		printf("t2 terminated\n");
	}
	
	if(pthread_join(t3, NULL)!=0){
		exit(EXIT_FAILURE);
	}
	else{
		printf("t3 terminated\n");
	}
	
	lhapply(sharedhash, print_person);
	
	createThread(&t4, tsearch, "zach");                                                                                                                                                               
  createThread(&t5, tsearch, "mikaela");
		
	if(pthread_join(t4, NULL)!=0){                                                                                                                                                                    
    exit(EXIT_FAILURE);                                                                                                                                                                             
  }                                                                                                                                                                                                 
  else{                                                                                                                                                                                             
    printf("t4 terminated\n");                                                                                                                                                                      
  }

	if(pthread_join(t5, NULL)!=0){                                                                                                                                                                    
    exit(EXIT_FAILURE);                                                                                                                                                                             
  }                                                                                                                                                                                                 
  else{                                                                                                                                                                                             
    printf("t4 terminated\n");                                                                                                                                                                      
  }

	tremove("zach");                                                                                                                                                                                  
  tremove("foster");                                                                                                                                                                                
  tremove("mikaela");

	*/

	
	printf("# of puts: %d\n", numberofPuts);
	printf("zach count: %d, foster count: %d, mikaela count: %d\n", zach, foster, mikaela);
	lhapply(sharedhash, delete_person);
	hclose(sharedhash);
  
	return 0;
	
}		 
