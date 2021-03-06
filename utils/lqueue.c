


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#include <pthread.h> 
#include <unistd.h>  

pthread_mutex_t m;

void unlockMutex(){
	pthread_mutex_unlock(&m);
	printf("shared queue mutex %p unlocked\n\n", (void*) &m);



	//fflush(stdout);
	//sleep(3);
}

void lockMutex(){
	if (pthread_mutex_trylock(&m)==0){
		printf("sharedqueue mutex %p locked\n", (void*) &m); 
	}
	else{
		printf("did not lock\n");
	}

	//printf("sharedqueue mutex %p locked\n", (void*) &m);

	//	fflush(stdout);
	//sleep(3);
}

/* create an empty queue 
 * # 
 */
queue_t* lqopen(){
	//printf("shared resource: %d", sharedresource);
	if (pthread_mutex_init(&m, NULL)!=0){
		printf("initialization of mutex for queue failed\n");
		return NULL;
	}
	lockMutex();
	printf("opening shared queue..\n");
	queue_t* queue = qopen();
	unlockMutex();
	return queue;
}


/* deallocate a queue, frees everything in it */
void lqclose(queue_t *qp){
	lockMutex();
	printf("closing shared queue...\n");
	qclose(qp);
	unlockMutex();
	pthread_mutex_destroy(&m);

}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(queue_t *qp, void *elementp){
	lockMutex();
	printf("putting element in shared queue...\n");
	//	sleep(5);
	qput(qp, elementp);
	unlockMutex();
	return 0;

}

/* get the first first element from queue, removing it from the queue */
void* lqget(queue_t *qp){
	lockMutex();
	printf("getting element in shared queue...\n");
	void* result = qget(qp);
	unlockMutex();
	return result;
}

/* apply a function to every element of the queue */
void lqapply(queue_t *qp, void (*fn)(void* elementp)){
	lockMutex();
	printf("applying function to shared queue...\n");
	qapply(qp, fn);
	unlockMutex();

}

/* search a queue using a supplied boolean function
 * skeyp -- a key to search for
 * searchfn -- a function applied to every element of the queue
 *          -- elementp - a pointer to an element
 *          -- keyp - the key being searched for (i.e. will be 
 *             set to skey at each step of the search
 *          -- returns TRUE or FALSE as defined in bool.h
 * returns a pointer to an element, or NULL if not found
 */
void* lqsearch(queue_t *qp, bool (*searchfn)(void* elementp, const void* keyp), const void* skeyp){
	lockMutex();
	printf("searching for element in shared queue...\n");
	void* found = qsearch(qp, searchfn, skeyp);
	unlockMutex();
	return found; 
}



				

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* lqremove(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp), const void* skeyp){
	lockMutex();
	printf("removing element from shared queue...\n");
	void* removed = qremove(qp, searchfn, skeyp);
	unlockMutex();
	return removed; 
}



