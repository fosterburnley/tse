
/* 
 * queue.c -- create a reusable queue module //
 */

#include <queue.h>                                                             
#include <stdio.h>                                                             
#include <stdlib.h>                                                            
#include <stdbool.h>                                                           
#include <string.h>                                                            
#include <inttypes.h>  

//#define DEBUG
/* the queue representation is hidden from users of the module */
typedef void queue_t;

//#define COUNT 3

/*
 * internal representation of queue
 */
typedef struct elementstruct{
	void *data; // an elementstruct 
	struct elementstruct* next;
} element_i;

typedef struct queuestruct{
	element_i *front;
	element_i *back;

} queue_i;

void* qget(queue_t *qp);

/* create an empty queue 
 *return the front pointer 
 */
queue_t* qopen(void){
	queue_i *rp;
	rp = (queue_i*)malloc(sizeof(queue_i));
	rp->front = NULL;
	rp->back = NULL;
	return (queue_t*)rp;
}

#ifdef DEBUG
typedef struct person{
	char name[10];
	int age;
	int gradYear;
  printf("print person in queue.c: name %s, age %d, gradyear %d", name, age, gradYear);
} person_t;
#endif
	

/* deallocate a queue, frees everything in it */

void qclose(queue_t *qp){
  queue_i *rp = (queue_i*) qp;
  //for(p = rp->front; p != NULL; p=p->next){
  //free(p);
  while(rp->front != NULL)(element_i *)qget(rp);
  free(rp);
}

int32_t qput(queue_t *qp, void *elementp){
  queue_i* rp;
  rp = (queue_i*) qp;
  element_i* elementp_i;
  element_i* elementp_t;
  
  elementp_i = (element_i*)malloc(sizeof(element_i));
  elementp_t = (element_i*)elementp;
	
  elementp_i->data = elementp_t;
  elementp_i->next = NULL;

  //make element the front
  if (rp->front == NULL){
    rp->front = elementp_i;
    rp->back = elementp_i;
  }
  
  //put the element in the back	
  else{
    rp -> back -> next = elementp_i;
    rp -> back = elementp_i;
  }

  return 0;
}

/* get the first first element from queue, removing it from the queue */
void* qget(queue_t *qp){
  element_i *got;
  queue_i *rp;
  void * vp;
  rp = (queue_i*) qp;
	if (rp->front==NULL){
		printf("getting from an empty queue\n\n");
		return NULL;
	}

  // store front in temporary pointer to return later 
  got = rp -> front;

  //make the original front equal to the orginal front element next element
  rp -> front = rp -> front -> next;

  // return the original front element through got pointer
  vp = got->data;
  free(got);
  return vp;

}

/* apply a function to every element of the queue */
void qapply(queue_t *qp, void (*fn)(void* elementp)){
	element_i* currentElement;
	void* currentElementV; 
	queue_i *rp;
	rp = (queue_i*) qp;

	currentElement= rp -> front;
	while (currentElement!=NULL){
		currentElementV = currentElement->data;
		if (currentElementV == NULL){
       			printf("warning: entered for loop but element is NULL! \n\n");
		}
		fn(currentElementV);
		currentElement=currentElement->next;
	}

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
void* qsearch(queue_t *qp, 
							bool (*searchfn)(void* elementp,const void* keyp),
							const void* skeyp){
	element_i *p;
	//	element_i *p_i;
	bool result;
  queue_i *rp = (queue_i*) qp; 
	// for loop to go through each element in the list
	for (p = rp->front; p != NULL; p=p->next){
		//put in the data of the current element, skeyp should only be the data already
		//compare elements in helper function
		result = searchfn(p->data, skeyp);

		//stop after finding a match
		if (result){
			return (void*)p->data;
		}
	}
	//if no match in the whole list 
	return NULL;
}

/* search a queue using a supplied boolean function (as in qsearch),
 * removes the element from the queue and returns a pointer to it or
 * NULL if not found
 */
void* qremove(queue_t *qp,
							bool (*searchfn)(void* elementp,const void* keyp),
							const void* skeyp){
	
	element_i *p;
	element_i *prev;
	element_i *got;
  bool result;
	queue_i *rp  = (queue_i*) qp; 
	void * vp;
	
	for (p = rp->front; p != NULL; p=p->next){
		result = searchfn(p->data, skeyp);

		//remove p if match found
		if (result==true){
			printf("removing element...");
			got=p;
			//check if p is front 
			if (p == rp->front){
			printf("removing front of queue...");
			rp ->front = p->next;
			}
			else{
			 prev ->next = p ->next;
			}
			vp = got->data;
			free(got);
			return vp;
		}
	     	// update back pointer
		prev = p;
	}
	printf("warning: no such element in queue\n");
	return NULL;
}

/* concatenatenates elements of q2 into q1
 * q2 is dealocated, closed, and unusable upon completion 
 */
void qconcat(queue_t *q1p, queue_t *q2p){
	//link back of q1 to front of q2
	queue_i *r1p = (queue_i*) q1p;
	queue_i *r2p = (queue_i*) q2p;
	//element_i* elementp_i;
	
	if (r1p->front==NULL && r2p->front==NULL){
		printf("both queues are empty... closing 2nd queue\n");
	}
	else if (r1p->front != NULL && r2p->front ==NULL){
		printf("the second queue is empty...closing the second queue\n");
	}
	else if (r1p->front == NULL && r2p ->front !=NULL){
		printf("concatenating where first queue is empty and second queue is not empty...\n");
		
		r1p->front = r2p->front;
		r1p->back = r2p->back;
		//		r1p->front->next= r2p->front->next;
	}
       		 
	else{
		printf("concatenating 2 queues where both queues are not empty...\n");
		r1p->back->next = r2p -> front;
		r1p->back=r2p->back;
	}
	free(r2p);

}

