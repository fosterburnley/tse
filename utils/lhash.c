
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

void unlockMutex(pthread_mutex_t * m){
  pthread_mutex_unlock(m);
}

void lockMutex(pthread_mutex_t * m){
  pthread_mutex_lock(m);
}

// hopen -- opens a hash table with the initial size hsize
hashtable_t* lhopen(uint32_t hsize, pthread_mutex_t* m){
  unlockedMutex(m);
  printf("opening hashtable...\n");
  hashtable_t* hash = hopen(hsize);
  lockMutex(m);
  return hash;
}


// hclose -- closes hash table
void lhclose(hashtable_t *htp, pthread_mutex_t* m){
  unlockMutex(m);
  printf("closing hashtable...\n");
  hclose(htp);
  lockMutex(m);
}

// hput -- puts an entry into a hash table under designated key
// returns 0 for sucess; nonzero otherwise
int32_t lhput(hashtable_t *htp, void *ep, const char *key, int keylen, pthread_mutex_t* m){
  unlockMutex(m);
  hput(htp, ep, key, keylen);
  lockMutex(m);
  return 0; 
}

// happly -- applies a function to every entry in the hash table
void lhapply(hashtable_t *htp, void(*fn)(void*ep), pthread_mutex_t* m){
  unlockMutex(m);
  happly(htp, fn);
  lockMutex(m);
}

void* lhsearch(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen, pthread_mutex_t* m){
  unlockMutex(m);
  printf("searching for element in hashtable...\n");
  void* found = hsearch(htp, searchfn, key, keylen);
  lockMutex(m);
  return found;
}

void* lhremove(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen, pthread_mutex_t* m){
  unlockMutex(m);
  void* removed = hremove(htp, searchfn, key, keylen);
  lockMutex(m);
  return removed;
}
