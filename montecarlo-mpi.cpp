#include<stdio.h>
#include<stdlib.h> //using rand etc
#include<mpi.h>
#include<time.h>
#include<math.h>
#include<iostream>
#define PI 3.14159265
#define rand_max 2147483647.0 

float randnum(float a, float b){
	//returns a random number between [a.b]
	float t = rand()/rand_max;
	return (t*b + (1-t)*a) ;
}

int partialcount(int n_){
	// number of sampling random points 
	int partialcount_ = 0;
	for(int i=0; i<n_; i++){
		float x = randnum(0,PI);
		float y = randnum(0,1);
		if(y>sin(x)){
			partialcount_ = partialcount_+1;
		}
	}
	return partialcount_;
}

int main(int argc, char** argv){

	const float n = 10000000; //number of sampling points/sub-intervals
	const float Area = PI;
	int count = 0;

	int my_PE_num;
	int size;
	MPI_Init(&argc, &argv);

	double t1 = MPI_Wtime();

	MPI_Comm_rank(MPI_COMM_WORLD, &my_PE_num);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	if(my_PE_num == 0){

		count = count + partialcount(n/size);

		MPI_Status status;
		int partialcount_;
		for(int j = 1; j<size; j++){
			MPI_Recv(&partialcount_, 1, MPI_INT, j, MPI_ANY_TAG, MPI_COMM_WORLD, &status) ;
			count = count + partialcount_;
		}
	}
	else{
		int partialcount__ = partialcount(n/size);
		MPI_Send( &partialcount__, 1, MPI_INT, 0, 10, MPI_COMM_WORLD); 
	}

	if(my_PE_num == 0){
		float integral = ((n-count)/n)*Area;
		printf("Integral by Monte-Carlo Method is %0.10f\n", integral);
		float error = 2 - integral;
		printf("Error is %0.10f \n", error);
	}
	if(my_PE_num==0){
		double t2 = MPI_Wtime();
		double runtime = (t2-t1);
		printf("Time is %f ms \n", 1000*runtime);
		printf("*****************************************************\n");
	}
	MPI_Finalize();
}