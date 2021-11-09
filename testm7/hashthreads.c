

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
#define HASHSIZE 10
#define MAXCHAR 10

hashtable_t* sharedhash;
pthread_mutex_t m;
int threadid = 1;

int counter = 0;

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
	printf("name %s, age %d, gradYear %d, fav %d \n\n", pp1->name, pp1->age, pp1->gradYear, pp1 ->fav);
	//printf("test");
}

static bool searchfn(void* elementp, const void* keyp){
	person_t* elementperson;
	elementperson = (person_t*)elementp;
	char *key = (char *)keyp;
	
	if(elementperson->name  == key){
	  //		printf("in if statement\n\n");
		return(true);
	}
	else return(false);
}

void createThread(pthread_t* thread, void* (fn)(coid* argp), void* argp){
  if (pthread_create(thread, NULL, fn, argp)!=0){
    printf("program terminated: could not create thread: %d\n", threadid);
    exit(EXIT_FAILURE);
  }
  else{
    //printf("within thread %d\n", threadid);
    //threadid++;
  }
}

void* tput(void* argp){
  char* personname = (char*) argp;
  person_t* person;
  int i = 0;
  while (i < 2){
    sleep(4);
    person = make_person(personname, 20, 2023, 500);
    printf("int 1 tput: %d\n", i);
    printf("putting %s in shared hash... \n", personname);
    lhput(sharedhash, (void*) person, &m); // need to double check these are correct format
    printf("printing hash...\n");
    lhapply(sharedhash, print_person, &m);
    i++;
  }
  return NULL;
}

void tget(){
  int i = 0;
  while(i < 8){
    printf("int i tget: %d\n", i);
    sleep(3);
    person_t* person = lhget(sharedhash, &m);
    printf("got person from shared hash...\n");
    print_person(person);
    i++;
    delete_person(person);
    lhapply(sharedhash, print_person, &m);
  }
}

void* tsearch(void* argp){
  char* personname = (char*) argp;
  int i = 1;
  while (i <10){
    sleep(2);
    printf("in ti tsearch: %d\n", i);
    person_t* found;
    printf("searching for %s from sharedhash...\n");

    found = (person_t*) lhsearch(sharedhash, searchfn, (void*)(personname), &m);
    if (found == NULL){
      printf("could not find person named %s in shared hash\n", personname);
    }
    else{
      printf("found person in shared hash: \n");
      print_person((void*)found);
    }
    i++;
    }
  return NULL;
}

void* tremove(void* argp){
  char* personname = (char*) argp;
  printf("removing person named %s from the shared hash..\n", personname);
  person_t* removed;
  removed = (person_t*) lhremove(sharedhash, searchfn, (void*)(personname), &m);
  if (removed == NULL){
    printf("could not remove person named %s in shared hash\n", personname);
    return NULL;
  }
  else{
    printf("removed person in shared hash: \n");
    print_person((void*) removed);
    printf("printing resulting shared hash after removal of %s: \n", personname);
    lhapply(sharedqueue, print_person, &m);
    delete_person(removed);
  }
  return NULL;
}

int main(){
	//printf("Made it into main function\n\n");
	person_t*steve = make_person("Steve", 50, 1999, 3);
	person_t*elise = make_person("elise", 80, 1963, 8);
	person_t*mike = make_person("mike", 53, 1963, 15);
	//person_t*pam = make_person("pam", 45, 1980, 30);
	//person_t*tom = make_person("tom", 30, 1990, 25);
  //person_t*jess = make_person("jess", 25, 2000, 17);                                           
	person_t* blank1;
	person_t* blank2;
	
  hashtable_t* hash1 = hopen(HASHSIZE);
	//printf("just opened a hastable\n\n");
	//print on empty hashtable
	happly(hash1, print_person);

	//remove from an empty hashtable
	blank1 = hremove(hash1, searchfn, mike->name, 4);
	//printf("just did happly\n\n");

	hput(hash1, (void*)(steve), steve->name, 5);
	hput(hash1, (void*)(elise), elise->name, 5);
	hput(hash1, (void*)(mike), mike->name, 4);
	
	
	//printf("just hput 1 people\n\n");
	happly(hash1, print_person); 
	printf("just before hsearch\n\n");

	blank1 = (person_t*)hsearch(hash1,searchfn,steve->name,5);
	print_person((void*)(blank1));
	printf("about to remove steve\n\n");
	blank2 = (person_t*)hremove(hash1,searchfn,mike->name,4);
	print_person((void*)(blank2));
	printf("printing those who are left\n\n");
	happly(hash1, print_person);
	hclose(hash1);

	

	// do not need to free the blanks bc they are pointing to same spot in memory as steve and mike 
	free(steve);
	free(elise);
	free(mike);
}
