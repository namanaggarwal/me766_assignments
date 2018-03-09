#include<stdio.h>
#include<stdlib.h> //using rand etc
#include<omp.h>
#include<time.h>
#include<math.h>
#define PI 3.14159265
#define rand_max 2147483647.0 //already defined in stdlib.h but still
float randnum(float a, float b){
	//returns a random number between [a.b]
	float t = rand()/rand_max;
	return (t*b + (1-t)*a) ;
}
int main(){
	double start = clock();
	//srand(time(NULL)); 
	//seeds the rand() function
	//sets the initial value for the rand() algorithm to generate pseudo-random numbers
	
	int n; //number of sampling points/sub-intervals
	scanf("%d", &n) ;

	float Area = PI;
	float count=0;
	int i;
	omp_set_num_threads(2); //change the number of threads to verify the code
	float x,y;
	#pragma omp parallel for default(none) shared(i,n) private(x,y) reduction(+:count)
	for(i=0; i<n; i++){
		x = randnum(0,PI);
		y = randnum(0,1);
		if(y>sin(x)){
			count = count + 1;}
	}
	
	float integral = ((n-count)/n)*Area;
	printf("Integral by Monte-Carlo Method is %0.10f\n", integral);
	float error = 2 - integral;
	printf("Error is %0.10f\n \n", error);
	double end = clock();
	double cputime = 1000*(end-start)/CLOCKS_PER_SEC;
	printf("Time is %0.3f ms \n", cputime);
}
