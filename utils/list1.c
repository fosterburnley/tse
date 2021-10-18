/* list1.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Thu Sep 30 23:36:28 2021 (-0400)
 * Version: 
 * 
 * Description: implement the list.h interface 
 * 
 */

#include <list.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAXREG 10

/* the representation of the car */
typedef struct car {
	struct car *next;
	char plate[MAXREG];
	double price;
	int year;
} car_t;

//list of cars
static car_t *front = NULL;

/*
 * put(); place a car at the beginning of the list
 * returns - if successful, nonzero otherwise
 * first make *front = car 
 * if the *front = null, then make the car in parameter next pointer = null
 * if not, then make car parameter next pointer = front car's original pointer 
 */

int32_t lput(car_t *cp){

	// front is now the parameter car

	// delcare new pointer
	car_t *p;

	// if list empty then make front the new car and point to null
	if (front == NULL){
		front = cp;
		cp -> next = NULL;
		
}

	// if list is not empty, make front = to the new car and make new car point to original 1st car
	
	else{
		car_t *p = front;
		front = cp;
		cp->next = p;
	}

	return 0;
}

/*
 * get(); remove and return the first car in the list; 
 * return NULL if the list is empty
 * if front = null then return null
 * if front is not null, then make front = front.next
 */
car_t *lget(){

	//if the list isn't empty 
	if (front != NULL){
		front = front ->next;  //remove the original front car 
		return front;
	}
	else {
		return NULL;
	}
}

/*
 * apply a function to every car in the list
 * use  afor loop and within each time, call the function and put the current car as the parameter of the function
 */

void lapply(void (*fn)(car_t *cp)){
	
	car_t *p; //declare pointer to car
	for (p = front; p != NULL; p->p.next){
		fn(p);
	}
}

/*
 * remove(); find, remove and return any car with the designated plate; 
 *return null if not present
 *
 * traverse through all cars
 * each time in for loop, compare the plate with the designated plate
 * once there is a match, remove the car
 * return the plate of the car

 * remove the car by declaring two pointers- f pointer will point to the previous car
 * once there is a match, make the f pointer point to the  next car of the car p is currently pointing to
 */
car_t *lremove(char *platep){

	car_t *f; //points to previous car
	car_t *p; //points to current car

	//traverse through list
	for (p=front; p!=NULL; p=p->next){
		//compare plates
		if (strcmp(p->plate, platep)){
			f->.next = p->next; //remove p
			return p->plate;
		}
		//update f
		f=p;
  }
	return NULL; //if exit the for loop and no plates match
}
		

