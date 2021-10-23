/* qtest.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Mon Oct  4 22:10:36 2021 (-0400)
 * Version: 
 * 
 * Description: A test of queue.c 
 * qtest 
 */

#define MAXREG 10

//#include <queue.c>
#include <queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>


typedef struct person {                                                       
  char name[MAXREG];                                                   
   int age;                                                               
   int gradYear;
	int fav;
	//int age;
} person_t;


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
	if (vp == NULL){
		printf("person not found \n\n");
		return;		
	}
	person_t* pp1;
	pp1=(person_t*) vp;
	
	//  printf("name: %s, gradYear = %d, fav = %d, age = %d \n\n", pp->name, pp->gradYear, pp->fav, pp->age);
	printf("name %s, age %d, gradYear %d, fav %d \n\n", pp1->name, pp1->age, pp1->gradYear, pp1 ->fav);
 
}

static bool searchfn(void* elementp, const void* keyp){                                                                             
// compare the two elements                                    
	 if(elementp == keyp){                                                       
		 return(true);                                                             
	   }else{                                                                      
	  return(false);                                                            
	 }                                                                           
}

int main(){
	queue_t *queue1; 
	queue_t *queue2;
  queue_t *queue3;
 queue_t *queue4;
 
	person_t *blank1;
	person_t *blank2;
	person_t *blank3;
	person_t *blank4;
	person_t *blank5;

	person_t *zach = make_person("zach", 20, 2023, 500);
	person_t *foster = make_person("foster", 22, 2022, 501); 
	 //	person_t *zach = make_person("zach", 2023, 100, 20);
	 //	person_t *foster = make_person("foster", 2022, 101, 22);
	person_t *mikaela = make_person("mikaela", 23, 2021, 502);
	person_t *cam = make_person("cam", 22, 2050, 503);
	person_t *sarah = make_person("sarah", 21, 2019, 504);
	person_t *billie = make_person("billie", 18, 2026, 505);

	/*
	// open/apply/put functions
	queue1 = qopen();
	//qapply(queue1,print_person); //print on empty list
	//printf("print before putting in queue \n");


	qget(queue1);
	// print_person((void*)zach);
	qput(queue1, (void*)zach);
	//printf("print first queue \n"); 
	//print_person((void*)zach);
 	qput(queue1, (void*)foster);
	qput(queue1, (void*)mikaela);
	qput(queue1, (void*)cam);
	//qput(queue1, (void*)sarah);
	//qput(queue1, (void*)billie); 
	printf("print first queue \n");
	qapply(queue1, print_person); //print on 3 person list
	//qclose(queue1);
	
	//put function
	queue2 = qopen();
	qput(queue2, (void*) cam);                                                         
	qput(queue2, (void*)sarah);                                                       
	qput(queue2, (void*)billie);
	printf("print second queue \n");
	qapply(queue2, print_person); //print on another 3 person list
	
	// get function
	blank1 = (person_t*)qget(queue1);
	blank2 = (person_t*)qget(queue1);
	blank3 = (person_t*)qget(queue1);
	printf("print queue1 after using qget() 3 times \n");
	qapply(queue1, print_person);
	//	printf("print queue2 after using qget()\n");
	//qapply(queue2, print_person);

	qput(queue1, (void*)foster);
	qput(queue1, (void*)mikaela);
	qput(queue1, (void*)cam);
	printf("print queue1 after using putting foster, mik, and cam back in()\n");
	qapply(queue1, print_person);
	// search for foster (present)
	blank1 = (person_t*)qsearch(queue1, searchfn, (void*)foster);
	printf("Person found after searching for foster\n");
	//print_person(blank1);
	print_person((void*)(blank1));
		
	//search for sarah (present);
	blank2 = (person_t*)qsearch(queue2, searchfn, sarah);
	printf("Person found after searching for sarah\n");
	//print_person(blank2);
	print_person((void*)(blank2));
	
	//search for mikaela (not present in queue2);
	blank3 = (person_t*)qsearch(queue2, searchfn, mikaela);
	printf("Person found after searching for mikaela\n\n");
	//print_person(blank3);
	print_person((void*)(blank3));
	
	//remove
	blank1 = (person_t*)qremove(queue1, searchfn, cam);
	printf("print queue1 after removing mikaela\n");
	qapply(queue1, print_person);
	
	blank2 = (person_t*)qremove(queue2, searchfn, sarah);
	printf("print queue2 after removing sarah\n");
	qapply(queue2, print_person);
	
	// remove billie from queue 2
	blank2 = (person_t*)qremove(queue2, searchfn, billie);                      
  printf("print queue2 after removing billie\n");                              
  qapply(queue2, print_person);

	// remove billie again from queue2 (not present)
	blank2 = (person_t*)qremove(queue2, searchfn, billie);                       
  printf("print queue2 after removing billie\n");                              
  qapply(queue2, print_person);
  
	//concat
	qconcat(queue1, queue2);
	printf("print concatenated queue\n");
	qapply(queue1, print_person);
	
	//close 
	qclose(queue1);
	//	qclose(queue2);

	*/
	
	// testing seg faults
	queue3 = qopen();
	qget(queue3);
	qput(queue3, (void*)zach);                                                                          
  qput(queue3, (void*)foster);                                                                          
  qput(queue3, (void*)mikaela);
	printf("print after putting zach, foster, mikaela in queue3\n\n");                                       
  qapply(queue3, print_person);

	qget(queue3);
	qget(queue3);
	qget(queue3);
	qget(queue3);
	qget(queue3);
	printf("print after getting zach, foster, mikaela + 2 more times in queue3\n\n");                                       
  qapply(queue3, print_person);
	
	//testing concat 
	queue4 = qopen();
	printf("concat testing all 4 cases \n\n");
	qconcat(queue3, queue4);
	printf("print after concatenating empty queue3 and empty queue4\n\n");                                       
  qapply(queue3, print_person);

	printf("reopen queue4...\n");
	queue4=qopen();
	printf("putting zach in queue 3...\n");
	qput(queue3, (void*)zach);
	printf("print after putting zach in queue3 with empty queue4\n");  
	qapply(queue3, print_person);
	qconcat(queue3, queue4);
	printf("print after concatenating queue3 and empty queue4 \n\n");
  qapply(queue3, print_person);                                                                                                    
	
	printf("reopen queue4...\n");
	queue4=qopen();
	printf("get() on queue3...now should be empty\n");
	qget(queue3);
	printf("putting zach in queue 4...\n");
	qput(queue4, (void*)zach);
	printf("print queue 4 after putting zach in queue3 with empty queue4\n");
	qapply(queue4, print_person);
	qconcat(queue3, queue4);
	printf("print queue 3after concatenating empty queue3 and queue4 with zach \n\n");
	qapply(queue3, print_person);                                                                                                    
          
	printf("reopen queue4...\n");
	queue4=qopen();
	printf("putting mikaela in queue 3...\n");
	qput(queue3, (void*)mikaela);
	printf("putting mikaela in queue 4...\n");
	qput(queue4, (void*)foster);
	qconcat(queue3, queue4);
	printf("print queue 3 after concatenating queue3 with mikaela and queue4 with foster \n\n");
	qapply(queue3, print_person);

	//testing remove 
	printf("remove() testing\n");
	printf("printing existing queue3\n");
	qapply(queue3, print_person);
	blank1 = (person_t*)qremove(queue3, searchfn, (void*)zach);
	printf("printing queue 3 after removing zach (1st element) from queue3\n\n");
	qapply(queue3, print_person);
	//	free (blank1);
	blank1 = (person_t*)qremove(queue3, searchfn, (void*)foster);
  printf("printing queue 3 after removing foster (last element) from queue3\n\n");                                                 qapply(queue3, print_person);
	//	free(blank1);
	
	printf("putting zach into queue3\n");
	printf("putting foster into queue3\n");
	qput(queue3, (void*)zach);
	qput(queue3, (void*)foster);
	printf("printing resulting queue3 after putting zach and foster\n");
	qapply(queue3, print_person);
  printf("removing zach from queue3\n");
	blank1 = (person_t*)qremove(queue3, searchfn, (void*)zach); 
  printf("printing resulting queue3 after removing zach\n\n");                                                            
  qapply(queue3, print_person);

	printf("removing sarah (not present)  from queue3\n");
	blank1 = (person_t*)qremove(queue3, searchfn, (void*)sarah);
	printf("printing resulting queue3 after removing sarah\n\n");
	qapply(queue3, print_person); 

	printf("removing mikaela (front) from queue3\n");
	blank2 = (person_t*)qremove(queue3, searchfn, (void*)mikaela);
	printf("printing resulting queue3 after removing mikaela\n\n");
	qapply(queue3, print_person); 
	
	printf("removing foster (last item) from queue3\n");
	blank3 = (person_t*)qremove(queue3, searchfn, (void*)foster);
	printf("printing queue3 after removing foster, the last item\n");
	qapply(queue3, print_person); 
	
	printf("trying to remove from an empty queue\n");
	blank4 = (person_t*)qremove(queue3, searchfn, (void*)cam);
	printf("printing queue3 after trying to remove from empty queue\n");
	qapply(queue3, print_person); 

	qclose(queue3);
	//	qclose(queue4);

	
	free(foster);
	free(cam);
	free(zach);
	free(mikaela);
	free(sarah);
	free(billie);
}
