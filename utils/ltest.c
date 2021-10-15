//<<<<<<< HEAD
/* 
 * ltest.c
 * first test of list implimentation by Zach Wong
 * /bin/Module3/ltest.c
 * first test of list implimentation by Zach Wong trying edits Mik
>>>>>>> d3e401b5704a036c019c39058cc4a8675ff00cc9
 * Change to push 10:44pm
 * Foster try to push edit
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <list.h>


/*
 * Make a car NEW EDIT MIK TESTING
 */
car_t* make_car(char* platep, int year, double price){
  car_t* cp;

  if(!(cp=(car_t*)malloc(sizeof(car_t)))){
    fprintf(stderr,"[Error: malloc failed allocating car]\n");
    return NULL;
  }
  cp->next=NULL;
  strcpy(cp->plate, platep);
  cp->year=year;
  cp->price=price;

  return cp;
}

/*                                                                                                                               
 * print out the cars in the list                                                                                        
 */  
int print_list(car_t *cp){
  car_t* p;
  car_t* n;
  printf("NEW LIST INBOUND:\n");
  for(p=cp; p!=NULL;p=p->next){
    if ((p->next) != NULL) {
      n=p->next;
      printf("plate: %s, year: %d, price: %lf, next: %s\n", p->plate, p->year, p->price, n->plate);
    }else printf("plate: %s, year: %d, price: %lf, next: NULL (end of list)\n", p->plate, p->year, p->price);
  }
  return 0;
}

/*
 * print out the specific car in the list 
 */
void print_car(car_t *cp){
  car_t*np;
  if ((cp->next) != NULL) {
      np=cp->next;
      printf("plate: %s, year: %d, price: %lf, next: %s\n\n", cp->plate, cp->year, cp->price, np->plate);
  }else printf("plate: %s, year: %d, price: %lf, next: NULL (end of list)\n\n", cp->plate, cp->year, cp->price);
  return;
}


int main(){
  car_t *mcqueen = make_car("Cachow", 2006, 100000.0);
  car_t *mater = make_car("Towmater", 1950, 10.50);
  car_t *speed = make_car("mach5", 1972, 35000.95);
  car_t *jeep = make_car("4SZF10", 2005, 3965.20);

  if (lremove(jeep->plate) == NULL) printf("CANNOT REMOVE, LIST IS EMPTY\n"); //remove on empty list
  if (lget() == NULL) printf("CANNOT GET, LIST IS EMPTY\n"); //get on empty list
  lapply(print_car);//apply on empty list
  lput(mcqueen);//put to an empty list
  lapply(print_car);//apply to non-empty list
  lput(mater);//put to non-empty list
  lapply(print_car);
  lput(speed);
  lput(jeep);
  lapply(print_car);

  printf("PRINTING RETURN VALUE OF GET FUNCTION:\n");
  print_car(lget());//get from a non-empty list (called inside of print_car to show successful returning of car)
  lapply(print_car); //show that get function successfully removed above car

  printf("REMOVED car: ");
  print_car(lremove("Towmater"));//remove on a non-empty list somewhere in the middle
  lapply(print_car);//show that above remove function actually removed the desired car

  lput(mater);
  lput(jeep);

  lapply(print_car);

  printf("REMOVING car at the beginning of the list:\n");
  print_car(lremove(jeep->plate));//remove on a non-empty list at the beginning
  lapply(print_car);

  printf("REMOVING car at the end of the list:\n");
  print_car(lremove("Cachow"));//remove on a non-empty list at the end
  lapply(print_car);

  

  free(mcqueen);
  free(mater);
  free(speed);
  free(jeep);
  exit(EXIT_SUCCESS);

}
