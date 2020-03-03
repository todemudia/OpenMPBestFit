/* Task Paralelism for line of best fit code by Triumph Joshua Odemudia */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 50000000


//global vars
double x[N];
double y[N];

//declare function for setting value of Y
double setY(int);

int main(void) {

    //Variables declarations
    double sumx;
    double sumy;
    double sumxsq;
    double sumxy;
	double start; //For timings
	int i, j;

	start = omp_get_wtime(); //Start timing
	#pragma omp parallel default(none) private(j) shared(x, y) 
	{
		#pragma omp single
		for (j = 0; j < N; j++){
			#pragma omp task
			{
				x[j] = j / 1003.01;
				y[j] = setY(x[j]);
			}//Task
		}//For loop
	}//Paralell region 

	#pragma omp parallel default(none) private(i) shared(x, y) \
	shared(sumx, sumy, sumxsq, sumxy)
	{
		#pragma omp single
		for (i = 0; i < N; i++){
			#pragma omp task
			{
				#pragma omp atomic
				sumx += x[i];
				#pragma omp atomic
				sumy += y[i];
				#pragma omp atomic
				sumxsq += x[i] * x[i];
				#pragma omp atomic
				sumxy += x[i] * y[i];
			}Task
		}//For loop
	}//Parallel region

	printf("Time: %f milliseconds\n",1000.*(omp_get_wtime()-start)); //Print time elapsed in paralell region

    //Calculate M
    double M = ((N*sumxy)-(sumx*sumy)) / ((N*sumxsq)-(sumx*sumx));

    //calculate C 
    double C = (sumy - (M*sumx)) / N;

	//Prints the value of M and C for best fit
	printf("The value of M is: %f\n",M);
	printf("The value of C is: %f\n",C);

}//Main


//Initializes the value of Y according to specified algorithm from assignment question
double setY(int val) {
    return (1.0/exp(val/50000.1) + sin(val/501.1) / (log(val+1.0E-06)+cos(val/499.01+val)));
}
