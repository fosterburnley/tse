/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <queue.h>
#include <hash.h>


#define MAXCHAR 10
#define MAXARR 2000

/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */
#define get16bits(d) (*((const uint16_t *) (d)))

typedef void hashtable_t; /* representation of a hashtable hidden */ 
//internal represenation of a hash table:
typedef struct elementstruct_h{//not too confident about this but think we need it for opening it...
  void *data; //can handle any sort of data
  char key[MAXCHAR];
} element_h;


typedef struct hashstruct{
  uint32_t hsize; //size of table as determined by argument from hopen
  uint32_t length; //number of elements in the hash table
  queue_t *slots[MAXARR];// hash slots--> this is an array
  
} hashtable_i;


static uint32_t SuperFastHash (const char *data,int len,uint32_t tablesize) {
  uint32_t hash = len, tmp;
  int rem;
  
  if (len <= 0 || data == NULL)
		return 0;
  rem = len & 3;
  len >>= 2;
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }
  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= data[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }
  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;
  //	printf("location of element = %d \n", hash % tablesize);
  return hash % tablesize;
}

// hopen -- opens a hash table with the initial size hsize

hashtable_t *hopen(uint32_t hsize){
  hashtable_i* hh;
	int i;
	hh = (hashtable_i*)malloc(sizeof(hashtable_i));// allocate memory for internal represenation of table
  //if(hh == NULL){
	// free(hh);
	// return NULL;
  //}
  hh->hsize = hsize; 
  hh->length = 0;
	for(i=0; i < hsize; i++){
		hh->slots[i]=qopen();
		//printf("queue open for slot %d", i);
		//if (hh->slots[i] == NULL){
		//	printf("null initialization in hopen");
		//}
	}
	
	//queue_t* slots [hsize]; //declare array
	//hh -> slots = slots; 
  //allocate memory for the array of entries
	// size of hh->slots is size of queue_t*
	//for (int i = 0; i < hh->hsize; i++){
	//	hh->slots[i] = qopen();
	//}
	//hh->slots = (queue_t*)calloc(hh->hsize, sizeof());
	//hh ->slots = slots;
  //if(hh->slots == NULL){
  //  free(hh);
  //  return NULL;
  //}
  return hh;
}


//hclose -- closes hash table
void hclose(hashtable_t *htp){

  hashtable_i*hh=(hashtable_i*)htp;
  // free any allocated keys
  for(uint32_t i = 0; i < hh->hsize; i++){
    //if (hh->slots[i] != NULL){
    //printf("deleting: %p\n\n",hh->slots[i]);//for testing purposes (prints pointer address)
      //free((void*)hh->slots[i]);
      qclose(hh->slots[i]);
		
    }
  
  
  // free entire array of slots
  //free(hh->slots);
  free(hh); //free table itself
}

// hput -- puts an entry into a hash table under designated key
// returns 0 for sucess; nonzero otherwise

int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen){
  uint32_t loc;
  queue_t*qp;
  hashtable_i*hh = (hashtable_i*)htp;
	
	// element_h*elep = NULL;
  loc = SuperFastHash(key, keylen, hh->hsize);
	//	printf("in hput, before first if statement\n\n");
	//printf("%p\n\n", hh->slots[loc]);
	/*if (hh->slots[loc]==NULL){//in the case that this is the first element at this location in the table
		printf("just inside if statement\n\n");
		qp = qopen();
		printf("opened new queue\n\n");
		hh->slots[loc]=qp;
    strcpy(elep->key, key);
    elep->data = ep;
    qput(qp, elep);
    hh->length+=1;
		printf("end of if statement\n\n");
		}else{//if there is an existing*/
	//printf("made it in else\n\n");
    qp = hh->slots[loc];//retrieve pointer to existing queue
		//printf("qp is %p\n\n", qp);
		//printf("key is %s\n\n", key);
		//printf("elep key is %s\n\n", elep->key);
		//strcpy(elep->key, key);//put key and data into element structure
		//	printf("after strcpy\n\n");
		//elep->data = ep;
		//printf("after eleb->data\n\n");
    qput(qp, ep);//put completed element structure into exisiting queue
    hh->length+=1;
		//printf("end of else statement\n\n");
		//}
  if(hh->slots[loc] != qp) return 1; //check that queue in proper part of the table
  //if(qsearch(qp, searchfn, elep) == NULL) return 1; //utilize search functionality of queue to check for success
	//	if(hh->slots[loc] == NULL){
	//	printf("error: slot is NULL at loc %d", loc);
	//	return 1;
	//}
	//printf("about to return 0\n\n");
	return 0; //if it makes it thru the above checks --> success
}


// happly -- applies a function to every entry in the hash table

void happly(hashtable_t *htp, void(*fn)(void*ep)){
  hashtable_i*hh = (hashtable_i*)htp;
  uint32_t loc;
	
	if(hh->length == 0) return;
	//	printf("past if statement, hhslotslo\n\n");
	for(loc=0; loc <hh->hsize;loc++){
		//		 printf("hhnumberofelements = %d \n\n", hh->length);
	  //printf("hhlocation = %d \n\n", loc);
		 //printf("hhinsideslot = %p \n\n", hh->slots[loc]);
			qapply(hh->slots[loc], fn);
		}	
}

//hsearch -- searchs for an entry under a designated key using a
//designated search fn -- returns a pointer to the entry or NULL if not found
//need to modify the search function because the elements of these queues
//will have a data field and a key field

/*static bool hsearchfn(void *elementp, const void *keyp){
  // compare key of each queue item against given key
  element_h *ep = (element_h *)elementp;
  char *key = (char *)keyp;
  printf("is %s equal to %s\n\n", ep->key, key); //test that ep->key is accessing the correct field of the element pointer
  if(ep->key == key) return true; //check if element's key field matches the desired key
  else return false;  
	}*/

void *hsearch(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
  uint32_t loc;
  hashtable_i*hh = (hashtable_i*)htp;
  queue_t*qp;//pointer to the queue to be searched
  //void *target;//original fed output of qsearch into this but no longer, not deleting yet incase we need it
  loc = SuperFastHash(key, keylen, hh->hsize); //run hash function so don't have to search entire array, just find location
  qp = hh->slots[loc];
  return qsearch(qp, searchfn, (void*)key);
  
}

//hremove -- removes and returns an entry under a designated key
// using a designated serach fn -- returns a pointer to the entry
// or null if not found
// basically the same logic as our hsearch:
// use hash function to find the location in the table
// then take the queue at that location and feed into queue implimentation of the function (in this case qremove)
// we think we needed a new a search fn so as to compare the key of the element to the search key

void *hremove(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
  uint32_t loc;
  hashtable_i*hh = (hashtable_i*)htp;
  queue_t*qp;//pointer to the queue to be searched
  //void *target;
  loc = SuperFastHash(key, keylen, hh->hsize); //run hash function so don't have to search entire array, just find location
  qp = hh->slots[loc];
  return qremove(qp, searchfn, (void*)key);
}
