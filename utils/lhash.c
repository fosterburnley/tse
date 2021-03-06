
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <queue.h>
#include <hash.h>
#include<pthread.h>
#include <unistd.h>

#define MAXCHAR 100
//#define MAXARR 2000

pthread_mutex_t m;  


void unlockhashMutex(){
 	printf("hash mutex %p unlocked\n\n", (void*)&m);
  pthread_mutex_unlock(&m);

}

void lockhashMutex(){
	if (pthread_mutex_trylock(&m)==0){                                                                                                                            
    printf("shared hash mutex %p locked\n", (void*) &m);                                                                                                        
  }                                                                                                                                                             
  else{                                                                                                                                                         
    printf("did not lock\n");                                                                                                                                   
  }    
	//printf("shared hash mutex %p locked\n", (void*)&m); 
  //pthread_mutex_lock(&m);
}


// hopen -- opens a hash table with the initial size hsize
hashtable_t* lhopen(uint32_t hsize){
  
	if ((pthread_mutex_init(&m, NULL) != 0)){
		printf("initialization of mutex for hash failed\n"); 
		return NULL;
	}
	lockhashMutex();
  printf("opening shared hashtable...\n");
  hashtable_t* hash = hopen(hsize);
  unlockhashMutex();
  return hash;
}


// hclose -- closes hash table
void lhclose(hashtable_t *htp){
  lockhashMutex();
  printf("closing shared hashtable...\n");
  hclose(htp);
  unlockhashMutex();
	pthread_mutex_destroy(&m);
}

// hput -- puts an entry into a hash table under designated key
// returns 0 for sucess; nonzero otherwise
int32_t lhput(hashtable_t *htp, void *ep, const char *key, int keylen){
  lockhashMutex();
  hput(htp, ep, key, keylen);
  unlockhashMutex();
  return 0; 
}

// happly -- applies a function to every entry in the hash table
void lhapply(hashtable_t *htp, void(*fn)(void*ep)){
  lockhashMutex();
  happly(htp, fn);
  unlockhashMutex();
}

void* lhsearch(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
  lockhashMutex();
  printf("searching for element in shared hashtable...\n");
  void* found = hsearch(htp, searchfn, key, keylen);
  unlockhashMutex();
  return found;
}

void* lhremove(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
  lockhashMutex();
  void* removed = hremove(htp, searchfn, key, keylen);
  unlockhashMutex();
  return removed;
}
