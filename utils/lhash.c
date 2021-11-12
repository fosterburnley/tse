
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
#define MAXARR 2000

pthread_mutex_t m;  


void unlockMutex(){
  pthread_mutex_unlock(&m);
}

void lockMutex(){
  pthread_mutex_lock(&m);
}

// hopen -- opens a hash table with the initial size hsize
hashtable_t* lhopen(uint32_t hsize){
  
	if ((pthread_mutex_init(&m, NULL) != 0)){
		printf("initialization of mutex for hash failed\n"); 
		return NULL;
	}
	lockMutex();
  printf("opening hashtable...\n");
  hashtable_t* hash = hopen(hsize);
  unlockMutex();
  return hash;
}


// hclose -- closes hash table
void lhclose(hashtable_t *htp){
  lockMutex();
  printf("closing hashtable...\n");
  hclose(htp);
  unlockMutex();
	pthread_mutex_destroy(&m);
}

// hput -- puts an entry into a hash table under designated key
// returns 0 for sucess; nonzero otherwise
int32_t lhput(hashtable_t *htp, void *ep, const char *key, int keylen){
  lockMutex();
  hput(htp, ep, key, keylen);
  unlockMutex();
  return 0; 
}

// happly -- applies a function to every entry in the hash table
void lhapply(hashtable_t *htp, void(*fn)(void*ep)){
  lockMutex();
  happly(htp, fn);
  unlockMutex();
}

void* lhsearch(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
  lockMutex();
  printf("searching for element in hashtable...\n");
  void* found = hsearch(htp, searchfn, key, keylen);
  unlockMutex();
  return found;
}

void* lhremove(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
  lockMutex();
  void* removed = hremove(htp, searchfn, key, keylen);
  unlockMutex();
  return removed;
}
