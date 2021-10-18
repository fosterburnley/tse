/* test1.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Sat Sep 25 18:39:33 2021 (-0400)
 * Version: 
 * 
 * Description: This is the first scaffold test for integrate.c 
 * 
 */

#include <integrate.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// check arguments for n and p flavors 

void checkArgsN(double a, double b, uint32_t n){
	if (a > b || n < 1){

		printf("illegal arguments: usage: integrate.c [lower limit] [upper limit] [-n] [specify]");
		exit (EXIT_FAILURE);
	}
}

void checkArgsP(double a, double b, double p){
	if (a > b || p < 0 || p > 1){

		printf("illegal arguments usage: integrate.c [lower limit] [upper limit] [-n] [specify]");
		exit (EXIT_FAILURE);
	}
}

// function to be integrated on                                                 
double f (double x) {                                                                 
  double y;                                                                     
  y = (3*(x*x) + 4*x + 5);                                                    
  //printf("y value  %lf \n", y);                                               
  return y;                                                                   
}

int main (int argc, char *argv[]){
	
  double a = 0;                                                                 
  double b = 0;                                                                 
  uint32_t n = 0; // non initialized value                                      
  double p = 0;  // non initialized value                                       
  double result = 0;

	//char aStr = *argv[1];
	//char bStr = *argv[2];
	//printf("char a %s", aStr);
	//char flagStr = *argv[3];
	//char numberStr = *argv[4];
	char* end;

	//check for right number of arguments

	if (argc != 5){
		printf("error count: usage: integrate.c [lower limit] [upper limit] [-n] [specify]");
		exit (EXIT_FAILURE);
	}
  
	a = strtod(argv[1], &end); 
	b = strtod(argv[2], &end);

	//make sure switches are appropriate

		if (strcmp(argv[3], "-n") != 0 && strcmp (argv[3], "-p") != 0){
		printf("illegal option: usage: integrate.c [lower limit] [upper limit] [-n or -p] [specify]");  
		}

  if (strcmp(argv[3], "-n") == 0){ 
	  	double (*fn)(double x); 
	  	fn = f;                         
			n = strtoul(argv[4], &end, 32);
		 checkArgsN(a, b, n);   
     integrate(fn, a, b, p, &result, &n);                                      
     printf("interval: [%lf-%lf], n: %d, result = %f\n", a, b, n, result);        
   } 

	if (strcmp(argv[3], "-p") == 0){ 
	 	double (*fn)(double x); 
	  fn = f; 
	 	double t = f(3); 
	 	printf("test f function %lf \n", t); 
		
	/* 	//printf("enter precision");                                                 */
	/* 	//scanf("%lf", &p);                                                          */
   
		p = strtod(argv[4], &end);
		  checkArgsP(a, b, p); 
		 integrate(fn, a, b, p, &result, &n);                                       
		 //calculateIntegralP(a, b, p, &result)
		
      printf("interval: [%lf-%lf], n: %d, result = %f\n", a, b, n, result);     
  }
		exit(EXIT_SUCCESS);
}


