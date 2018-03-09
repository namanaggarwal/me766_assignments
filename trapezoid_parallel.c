#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#include<math.h>
#define PI 3.14159265

int main(){
	//n is the number of sub-intervals in this method
	float start = clock();
	int n;
	scanf("%d",&n);

	double a,b,h,sum;
	a = 0;
	b = PI;
	h = (b-a)/n;
	sum=0;

	int i;
	omp_set_num_threads(2); //change the number of threads to verify the code

	#pragma omp parallel for default(none) shared(i,n,a,h) reduction(+:sum)
	for(i=0; i<n; i++){
		float ti, ti1;
		ti = a + i*h;
		ti1 = ti + h;
		sum = sum + (h/2)*(sin(ti) + sin(ti1)) ;
	}
	
	printf("Trapezoid Integral is %0.10f \n", sum);
	float end = clock();
	float cputime = 1000*(end-start)/CLOCKS_PER_SEC;
	printf("Time is %f ms \n", cputime);
}
