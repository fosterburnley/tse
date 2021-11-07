/* threads.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Sat Nov  6 15:06:39 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#include "../utils/lqueue.c"
#include <unistd.h>
#include <string.h>

#define MAXREG 15

// declare shared queue and mutex 
queue_t* sharedqueue;
pthread_mutex_t m;
int dummy = 0;

typedef struct person {                                                                                                                                                              
  char name[MAXREG];                                                                                                                                                                 
   int age;                                                                                                                                                                          
   int gradYear;                                                                                                                                                                     
  int fav;                                                                                                                                                                           
  //int age;                                                                                                                                                                         
} person_t;

void delete_person(void* person_v){

	person_t* person = (person_t*) person_v;
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
  //  strcpy(pp->fav, fav);                                                                                                                                                          
  pp->age = age;                                                                                                                                                                     
  pp->gradYear = gradYear;                                                                                                                                                           
  pp -> fav = fav;                                                                                                                                                                   
  //  printf("testing print name %s, gradYear %d, fav %d, age %d \n\n", pp->name, pp->gradYear, pp->fav, pp->age);                                                                   
  //printf("make person name %s, age %d \n\n", pp->name, pp->age);                                                                                                                   
  return pp;                                                                                                                                                                         
                                                                                                                                                                                     
}    

                                                                                                                                                                                     
/*                                                                                                                                                                                   
 * print the data of the person type                                                                                                                                                 
 * return if successful                                                                                                                                                              
 */                                                                                                                                                                                  
                                                                                                                                                                                     
void print_person(void *vp){                                                                                                                                                         
  if (vp == NULL){                                                                                                                                                                   
    printf("person not found \n\n");                                                                                                                                                 
    return;                                                                                                                                                                          
  }                                                                                                                                                                                  
  person_t* pp1;                                                                                                                                                                     
  pp1=(person_t*) vp;                                                                                                                                                                
                                                                                                                                                                                     
  //  printf("name: %s, gradYear = %d, fav = %d, age = %d \n\n", pp->name, pp->gradYear, pp->fav, pp->age);                                                                          
  printf("name %s, age %d, gradYear %d, fav %d \n\n", pp1->name, pp1->age, pp1->gradYear, pp1 ->fav);                                                                                
                                                                                                                                                                                     
}


void *tfunc1(void* argp){
	//dummy = 1;
	printf("within thread 1 \n");
	fflush(stdout);
	
	//queue_t* sharedqueue = (queue_t*) argp;
	
	person_t *zach= make_person("zach", 20, 2023, 500);
	
	printf("putting person zach in shared queue... \n"); 
	lqput(sharedqueue, (void*)zach, &m);
	printf("printing queue...\n");
	lqapply(sharedqueue, print_person, &m);
	//printf("deleting people in queue...\n");
	
	//lqapply(sharedqueue, delete_person, &m);
	
	//printf("printing queue...\n");
	//lqapply(sharedqueue, print_person, &m); 
	return NULL;
}

void* tfunc2(void* argp){

	printf("within thread2 \n");
	fflush(stdout);

	printf("getting from shared queue...\n");
	person_t* person = lqget(sharedqueue, &m);
	printf("printing person after getting from shared queue...");
	print_person(person);
	lqapply(sharedqueue, print_person, &m);
	return NULL;
}

int main(){
	// declare threads and initialize mutex
	//	printf("test");
	pthread_t t1, t2;
	pthread_mutex_init(&m, NULL);

	//make people

	
	// initilize shared queue
	sharedqueue = lqopen(&m);

	// create the threads
	
  if (pthread_create(&t1, NULL, tfunc1, NULL)!=0){
		printf("program terminated: could not create thread\n");
		exit(EXIT_FAILURE);
	}
	// wait until t1 is done
	if (pthread_join(t1, NULL)!=0){
		exit(EXIT_FAILURE);
	}
	
	if (pthread_create(&t2, NULL, tfunc2, NULL)!=0){                                                                                                                                   
	   printf("program terminated: could not create thread\n");                                                                                                                        
		 exit(EXIT_FAILURE);                                                                                                                                                             
  }

	if (pthread_join(t2, NULL)!=0){                                                                                                                                                    
    exit(EXIT_FAILURE);                                                                                                                                                              
  }  
	
	
	
	/*
	if (pthread_join(t2, NULL)!=0){                                                                                                                                                 
		// printf("within thread 1\n");                                                                                                                                                  
    exit(EXIT_FAILURE);                                                                                                                                                              
	}      
	*/
	// destroy mutex after all threads terminated
	/*
	printf("closing queue...\n");
  lqclose(sharedqueue, &m);
	*/
	pthread_mutex_destroy(&m);

	
	//printf("dummy after create thread: %d\n", dummy);
	return 0;
}
