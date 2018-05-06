#include<iostream>
#include<mpi.h>
#include<math.h>
#include<time.h>

#define PI 3.14159265

double integral(double start, double end, int n){
	//n is the number of sub-intervals
	double sum=0;
	double h = (end-start)/n;
	for(int i=0; i<n; i++){
		double ti = start + i*h;
		double ti1 = ti + h;
		sum = sum + (h/2)*(sin(ti) + sin(ti1)) ;
	}
	return sum;
}

int main(int argc, char** argv){
	//n is the number of sub-intervals in this method
	const int n=10000000;
	const double a = 0;
	const double b = PI;
	const double h = (b-a)/n;
	double sum=0;	

	int my_PE_num;	
	int size;
	MPI_Init(&argc, &argv);

	double t1 = MPI_Wtime();
	for(){
		
	}
	MPI_Comm_rank(MPI_COMM_WORLD, &my_PE_num); 
	MPI_Comm_size(MPI_COMM_WORLD, &size); /* stores the number of initiated processes in the variable 'size' */

	//compute the partial sum and send the partial sum to the master process
	double chunkstart = a + my_PE_num*(b-a)/size ;
	double chunkend = chunkstart + (b-a)/size ;

	if(my_PE_num == 0){
		//receive the partial sums from each process and sum all of them to get the required integral
		MPI_Status status;
		sum = sum + integral(chunkstart, chunkend ,n/size);
		for(int j = 1; j<size; j++){
			double partialsum;
			MPI_Recv(&partialsum, 1, MPI_DOUBLE, j, MPI_ANY_TAG, MPI_COMM_WORLD, &status) ;
			sum = sum + partialsum;
		}
	}
	else{
		double partialintegral = integral(chunkstart, chunkend ,n/size);
		MPI_Send( &partialintegral, 1, MPI_DOUBLE, 0, 10, MPI_COMM_WORLD) ;
	}

	if(my_PE_num == 0){
		printf("Number of Processes = %d \n", size);
		printf("Trapezoid Integral is %0.10f \n", sum);
		printf("Error is %f percent \n", 50*(2 - sum));
	}
	
	double t2 = MPI_Wtime();
	double runtime = (t2-t1);
	
	if(my_PE_num==0){
		printf("Time is %f ms \n", 1000*runtime);
		printf("*****************************************************\n");
	}
	
	MPI_Finalize();
}