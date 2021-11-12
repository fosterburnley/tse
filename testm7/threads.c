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
//pthread_mutex_t m;
int threadid = 1;

int counter = 0;

int zach = 0;
int foster = 0;
int mikaela=0;
typedef struct person {                                                                                                                                                              
  char name[MAXREG];                                                                                                                                                                 
   int age;                                                                                                                                                                          
   int gradYear;                                                                                                                                                                     
  int fav;                                                                                                                                                                           
  //int age;                                                                                                                                                                         
} person_t;

// delete the person
void delete_person(void* person_v){

	person_t* person = (person_t*) person_v;
	printf("freeing person: %s\n", person->name);
	free(person);

}

// make the person
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

void createThread(pthread_t* thread, void* (fn)(void* argp), void* argp){
if (pthread_create(thread, NULL, fn, argp)!=0){                                                                                                                                                        
	printf("program terminated: could not create thread: %d\n", threadid);
	//fflush(stdout);
	exit(EXIT_FAILURE);                                                                                                                                                                                   
  }                                                                                                                                                                                                        
 else{                                                                                                                                                                                                    
	 //printf("within thread %d\n", threadid);
	 //threadid++;
 } 

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
	//fflush(stdout); 
}

//make person and put in queue
void *tput(void* argp){
	char* personname = (char*) argp;
	person_t *person; 
	//person_t *foster = make_person("foster", 22, 2022, 501);
	//person_t *mikaela = make_person("mikaela", 23, 2021, 502);
	int i = 0;
	while (i < 100){
		//sleep(5);
		person = make_person(personname, 20, 2023, 500);
		//printf("int i tput: %d\n", i); 
		printf("putting %s in shared queue... \n", personname);
		//fflush(stdout);
		lqput(sharedqueue, (void*)person);
		printf("printing queue...\n");
		lqapply(sharedqueue, print_person);
		//printf("deleting people in queue...\n");
	
	//lqapply(sharedqueue, delete_person, &m);
	
	//printf("printing queue...\n");
	//lqapply(sharedqueue, print_person, &m);
		i++;
	}
	return NULL;
}

// get from queue 
void tget(){
 
	int i = 0;
	while(i < 350){
		printf("int i tget: %d\n", i); 
		//sleep(10);
		//printf("getting from shared queue...\n");
		//fflush(stdout);
		
		person_t* person = lqget(sharedqueue);
		printf("got person from shared queue...");
		//fflush(stdout); 
		//print_person(person);
		i++;
		if (strcmp(person->name, "zach") == 0){
			zach++;
		}
		else if (strcmp(person->name, "foster") == 0){
			foster++;
		}
		else if (strcmp(person->name, "mikaela") ==0){
			mikaela++;
		}
		delete_person(person);
		//		lqapply(sharedqueue, print_person);
	}

	//	return NULL;
}

bool searchfn(void* elementp, const void* keyp){
	person_t* currentperson = (person_t*) elementp;
	char* personname = (char*) keyp;
	if(strcmp(currentperson->name, personname)==0){
		return true;
	}
	else{
		return false; 
	}


}

// search  from queue
// argp should be person's name 
void* tsearch(void* argp){
	//person name as key
	char* personname = (char*) argp;
	int i = 1;
	while (i < 10){
		//		sleep(4);
		printf("in ti tsearch: %d\n", i);
		person_t* found;
		printf("searching for %s from sharedqueue...\n", personname);
		
		found = (person_t*) lqsearch(sharedqueue, searchfn, (void*)(personname));
		if (found == NULL){
			printf("could not find person named %s in shared queue\n", personname);
		}
		else{
			printf("found person in shared queue: \n");
			print_person((void*)found);
		}
		i++;
	}
	
	return NULL;
}

void* tremove(void* argp){
	int i = 0;
	while (i < 100){
		char* personname = (char*) argp;
		printf("removing person named  %s from sharedqueue...\n", personname);
		person_t* removed;
		removed  = (person_t*) lqremove(sharedqueue, searchfn, (void*)(personname));
		if (removed == NULL){
			printf("could not remove person named %s in shared queue\n", personname); 
			return NULL;
		}
		else{
			printf("removed person in shared queue: \n");
			print_person((void*) removed);
			printf("printing resulting shared queue after removal of %s: \n", personname);
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
			lqapply(sharedqueue, print_person);
			delete_person(removed);
		}
		i++;
	}
	 return NULL;
} 


int main(){
	// declare threads and initialize mutex
	//	printf("test");
	pthread_t t1  = 0;
	pthread_t t2 = 0;
	pthread_t t3 = 0;
	pthread_t t4 = 0;
	pthread_t t5 = 0;
	//	pthread_mutex_init(&m, NULL);

	//make people

	
	// initilize shared queue
	sharedqueue = lqopen();

	// create the threads
	createThread(&t1, tput, "zach");
	createThread(&t2, tput, "foster");
	createThread(&t3, tput, "mikaela"); 
	//	createThread(&t3, tsearch, "zach");
	//createThread(&t4, tsearch, "mikaela");
	/*
	// wait until t1 is done
	if (pthread_join(t1, NULL)!=0){
		exit(EXIT_FAILURE);
	}
	*/
	
	//createThread(&t2, tget, NULL);
	/*
	createThread(&t3, tput, NULL);
	createThread(&t4, tsearch, (void*) personname);
	createThread(&t5, tremove, (void*) personname);   
	*/

	
	if (pthread_join(t1, NULL)!=0){                                                                                                                                                            
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
	lqapply(sharedqueue, print_person);     
	//tget();
	
	
	tremove("zach");
	tremove("foster");
	tremove("mikaela");
	lqclose(sharedqueue);

	printf("zach count: %d, foster count: %d, mikaela count: %d\n", zach, foster, mikaela);
	
	
	/*
	if (pthread_join(t3, NULL)!=0){
		exit(EXIT_FAILURE);
	}
	else{                                                                                                                                                                                    
    printf("t3 terminated\n");                                                                                                                                                                
  }
	*/
	/*
	if(pthread_join(t4, NULL)!=0){
		exit(EXIT_FAILURE);
	}
	else{                                                                                                                                                                                      
    printf("t4 terminated\n");                                                                                                                                                                
  } 
	*/
	/*
	if (pthread_join(t2, NULL)!=0){                                                                                                                                                                          
    exit(EXIT_FAILURE);                                                                                                                                                                                    
  } 
	*/
	  
	//	pthread_exit();
	/*
	if (pthread_join(t3, NULL)!=0){                                                                                                                                                                          
    exit(EXIT_FAILURE);                                                                                                                                                                                    
  }   
	if(pthread_join(t4, NULL)!=0){                                                                                                                                                                           
    exit(EXIT_FAILURE);                                                                                                                                                                                    
  }           
	if (pthread_join(t5, NULL)!=0){                                                                                                                                                                          
		exit(EXIT_FAILURE);                                                                                                                                                                                    
  } 
	
	*/
	/*

	// put zach back in
	createThread(&t3, tput, NULL);
	if (pthread_join(t2, NULL)!=0){
		exit(EXIT_FAILURE);
	}

	//search for zach

	 
	createThread(&t4, tsearch, (void*) personname);
	if(pthread_join(t3, NULL)!=0){                                                                                                                                                                          
    exit(EXIT_FAILURE);                                                                                                                                                                                    
  }   
	
	createThread(&t5, tremove, (void*) personname);
	if (pthread_join(t4, NULL)!=0){  
    exit(EXIT_FAILURE);                                                                                                                                                                                   
  }
	
	*/
	/*
	if (pthread_join(t2, NULL)!=0){                                                                                                                                                 
		// printf("within thread 1\n");                                                                                                                                                  
    exit(EXIT_FAILURE);                                                                                                                                                              
	}      
	*/
	// destroy mutex after all threads terminated
	//lqapply(sharedqueue, print_person, &m);
	/*
	printf("deleting people in queue...\n");
	lqapply(sharedqueue, delete_person, &m);
	
  lqclose(sharedqueue, &m);
	
	pthread_mutex_destroy(&m);
	*/
	
	//printf("dummy after create thread: %d\n", dummy);
	return 0;

}
