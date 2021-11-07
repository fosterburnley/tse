


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#include <pthread.h> 
#include <unistd.h>  


void unlockMutex(pthread_mutex_t* m){
	pthread_mutex_unlock(m);
	printf("mutex %p unlocked\n", (void*) m);
	fflush(stdout);
	sleep(3);
}

void lockMutex(pthread_mutex_t* m){
	pthread_mutex_lock(m);
	printf("mutex %p locked\n\n", (void*) m);
	fflush(stdout);
	sleep(3);
}

/* create an empty queue 
 * # 
 */
queue_t* lqopen(pthread_mutex_t* m){
	//printf("shared resource: %d", sharedresource);
	unlockMutex(m);
	printf("opening queue..\n");
	queue_t* queue = qopen();
	lockMutex(m);
	return queue;
}


/* deallocate a queue, frees everything in it */
void lqclose(queue_t *qp, pthread_mutex_t* m){
	unlockMutex(m);
	printf("closing queue...\n");
	qclose(qp);
	lockMutex(m);  

}

/* put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(queue_t *qp, void *elementp, pthread_mutex_t* m){
	unlockMutex(m);
	printf("putting element in queue...\n");
	qput(qp, elementp);
	lockMutex(m);
	return 0;

}

/* get the first first element from queue, removing it from the queue */
void* lqget(queue_t *qp, pthread_mutex_t* m){
	unlockMutex(m);
	printf("getting element in queue...\n");
	void* result = qget(qp);
	lockMutex(m);
	return result;
}

/* apply a function to every element of the queue */
void lqapply(queue_t *qp, void (*fn)(void* elementp), pthread_mutex_t* m){
	unlockMutex(m);
	printf("applying function to queue...\n");
	qapply(qp, fn);
	lockMutex(m);

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
void* lqsearch(queue_t *qp, bool (*searchfn)(void* elementp, const void* keyp), const void* skeyp, pthread_mutex_t* m){
	unlockMutex(m);
	printf("searching for element in queue...\n");
	void* found = qsearch(qp, searchfn, skeyp);
	lockMutex(m);
	return found; 
}



				

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* lqremove(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp), const void* skeyp, pthread_mutex_t* m){
	unlockMutex(m);
	printf("removing element from queue...\n");
	void* removed = qremove(qp, searchfn, skeyp);
	lockMutex(m);
	return removed; 
}



