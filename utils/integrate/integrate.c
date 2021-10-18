/* integrate.c --- 
 * 
 * 
 * Author: Zachary J. Wong
 * Created: Mon Sep 20 23:53:43 2021
 * Version: 1.0
 * 
 * Description: This program calculates the integral of a particular function  
 * 
 */

//main
// declare local variables for a, b, n, and result
// set values of a b and n directly in main program
// compute resulting area (result)
// print the result
// return value of 0

// first y = 2x + 3

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "integrate.h"
#include <math.h>

// check for illegal arguments and wrong input
// if a is greater than b or pNum is not between 0 and 1 or number strips is negative, exit failure
void checkArgs (double aNum, double bNum, uint32_t *nNum, double pNum){
	if (aNum > bNum || pNum > 1 || pNum < 0 || *nNum < 0){
			exit (EXIT_FAILURE);
		}
}

	

// function to be integrated on
//double f () {
//double y;
//	y = (2*(x*x) + 9*x + 4);
	//printf("y value  %lf \n", y);
	//return y;
//}

void calculateIntegral (double aNum, double bNum, uint32_t nNum, double *result, double (*fn)(double x));
	
// this function calls calculate integral the necessary amount of times and stops it after the current result- previous result <= precision value
void calculateIntegralP (double aNum, double bNum, uint32_t *nNum, double pNum, double *result, double (*fn)(double x)){

	// first assignment of number of strips is 1
	*nNum = 1;
	double pastResult = 0;
	// initialize result difference to be bigger than 1
	double resultDifference = 2;
	
	while (fabs(resultDifference) > pNum){
		*result = 0;
		calculateIntegral(aNum, bNum, *nNum, result, fn); // result stored in *result

		// update nNum
		*nNum = *nNum * 2;
		//		printf("number of strips %d \n", *nNum);  
		//printf("result %lf \n", *result);
		//printf("past result %lf \n", pastResult);
		// update result difference
		resultDifference = *result - pastResult;
		//printf("result difference %lf", resultDifference);
		//update current result ot past result
		pastResult = *result;
		//printf("updated past result %lf \n", pastResult);
}
}

//store result in address of result
// get each input and output and store in variables through the f function
// for the number of strips
// end point of each strip is add (a+b)/n to a
// update the total (result variable) 
void calculateIntegral (double aNum, double bNum, uint32_t nNum, double *result, double (*fn)(double x)){
	double aInput = aNum; //x value of a
	double aOutput; //y value of a
	double bOutput; // y value of b
	double bInput = bNum; // x value of b
	double aStrip; // area under ith strip
	double widthStrip; //how wide the strips ares 
 
	
	widthStrip = (bNum - aNum) / ((double) nNum);
	//printf("widthstrip %lf \n", widthStrip); 
	//printf("b-aOutput %lf \n", bInput-aInput); 
	//printf("aNum %lf \n", aNum);
	//printf("bNum %lf \n", bNum);
	//printf("nNum inside:  %d \n", nNum); 
	// runs the number of strips there are 
		for (int i = 0; i < nNum ; i++){	
			aOutput = fn(aInput);
			//printf("aInput  %lf \n", aInput);  
			bInput = widthStrip + aInput; // get the x value of end of rectan
			//printf("bInput  %lf \n", widthStrip + aInput);
			bOutput = fn(bInput);
			//printf("aOutput %lf \n", aOutput);
			//printf("bOutput %lf \n", bOutput);
			//printf("widthStrip %lf \n", widthStrip); 
			aStrip = (0.5 * (aOutput + bOutput)) * widthStrip;
			//printf("aStrip %lf", aStrip);
			*result = *result + aStrip;

	
			//update a and b
			aInput = bInput;
			bInput = bInput + widthStrip;
	
		}
		//printf("aOutput  %lf \n", aOutput);
		//printf("bOutput  %lf \n", bOutput); 
		//printf("result inside: %lf \n", *result);
}

int32_t integrate(double (*f)(double x),                                                                                                                   
                  double a, double b, double p,                                                                                                            
                  double *rp, uint32_t *sp){
	
	if (p == 0){
		calculateIntegral(a, b, *sp, rp, f);
	}

	if (*sp == 0){
		calculateIntegralP(a, b, sp, p, rp, f);  
	}
	
}
                                               

// run 
//int main (int argc, char *argv[])

//{
//	double a = 0;
//double b = 0;
//uint32_t n = 0; // non initialized value
	//double p = 0;  // non initialized value 
//double result = 0;

//printf("enter the lower limit ");
//scanf("%lf", &a);
//printf("enter a second lower limit ");
//scanf("%lf", &b);
	
//if (strcmp(argv[1], "n" == 0){
//	printf("enter number of splits ");
//	scanf("%d", &n);
//	checkArgs(a, b, &n, p); 
		//calculateIntegral(a, b, &n, &result);
//	integrate(f, a, b, p, &result, &n);
//	printf("interval: [%lf-%lf], n: %d, result = %f\n", a, b, n, result);  
//}
//
//	if (strcmp(argv[1], "p") == 0){
	//		printf("enter precision");
//		scanf("%lf", &p);
//		checkArgs(a, b, &n, p);
//		integrate(f, a, b, p, &result, &n); 
//		//calculateIntegralP(a, b, p, &result)
//		printf("interval: [%lf-%lf], n: %d, result = %f\n", a, b, n, result);  	
//	}
 
//  exit (EXIT_SUCCESS);
		
// }
		





