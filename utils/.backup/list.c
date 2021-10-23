/* list.c

Author: Zachary J. Wong, Foster Burnley, Mikaela O'Brien, Camden Parker

10/1/2021

adaption/manual copy of list1.c by Zachary J. Wong


*/

#include <list.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define MAXREG 10

/*the representation of the car */
/* typedef struct car {
  struct car *next;
  char plate[MAXREG];
  double price;
  int year;
}car_t;

*/

//list of cars
static car_t *front=NULL;

/*
* put(); place a car at the beginning of the list
* returns 0 if successful; nonzero otherwise
* first make *front = car
* if the *front = NULL, then make the car in parameter next pointer = NULL
* if not, then make the car parameter next pointer = front car's original pointer
*
*/

int32_t lput(car_t *cp){
  // front is now the parameter car

  //declare new pointer
  car_t *p;

  // if the list is empty then make front the new car and point it to null
  if (front == NULL){
    front=cp;
    cp->next=NULL;
  }
  // if the list is not empty, make front = to the new car and make new car point to origial 1st car
  else{
    p=front;
    front=cp;
    cp->next=p;
  }
  return 0;
}


/*
* get(); remove and return the first car in the list;
* return NULL if the list is empty
* if front=NULL then return NULL
* if front != NULL then return NULL
* 
*
*/

car_t *lget(){
  //if the list isn't empty
  if (front != NULL){
    car_t*got;
    got=front;
    front=front->next; //remove the original front car
    return got;
  }
  else{
    return NULL;
  }
}

/*
 * apply a function to every car in the list
 * use a for loop and within each time, call the function and put the current car as the parameter of the function
 */

void lapply(void(*fn)(car_t *cp)){
  car_t *p; //declare pointer to car
  if (front == NULL){
    printf("LIST EMPTY: NOTHING TO APPLY FUCTION TO\n");
  }
  else{
    printf("APPLYING FUNCTION TO LIST WITH FRONT: %s\n", front->plate);
    for (p=front;p!=NULL; p=p->next){
      fn(p);   
    }
  }
}

/*
 * remove(); find, remove and return any car with the designated plate;
 * return NULL if not present
 * 
 * traverse through all cars
 * each time in for loop, compare the plate with the designated plate (from argument)
 * once there is a match, remove the car
 * return the plate of the car
 *
 *remove the car by declaring two pointers-f pointer will point to the previous car
 * once there is a match, make the f pointer point to the car that car p is currently pointing to
 *
 */

car_t *lremove(char *platep){

  car_t *f; //points to previous car
  car_t *p; //points to current car

  //traverse through list
  for (p=front; p!=NULL; p=p->next){
    //compare plates
    if (strcmp(p->plate,platep)==0){
      if (p == front){ // edge case where removing first element in the list
	front = p -> next;
      }else{
      f->next = p->next; //remove p
      }
      return p;
    }
    //update
    f=p;
  }
  return NULL; //if exit the for loop and no plates match
}
