#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 50000000

// global vars
double x[N];
double y[N];

double setY(int);

int main(void) {


    //Variables declarations
    double sumx;
    double sumy;
    double sumxsq;
    double sumxy;
	double start;
	int i, j;

	start = omp_get_wtime(); //Timing starts

	#pragma omp parallel for default(none) shared(x, y) private(j)
    for (j = 0; j < N; j++) { 	/*Initializes the value of X according to specified algorithm from assignment question*/
        x[j] = j / 1003.01;
        y[j] = setY(x[j]); //Calls function to set the value of Y
     }// End of parallel region


	#pragma omp parallel for default(none) shared(x, y) private(i) \
	reduction(+:sumx) reduction(+:sumy) reduction(+:sumxsq) reduction(+:sumxy) 
    for (i = 0; i < N; i++){
        sumx += x[i];
        sumy += y[i];
        sumxsq += x[i] * x[i];
        sumxy += x[i] * y[i];
    }//End of paralell region

	printf("Time: %f milliseconds\n",1000.*(omp_get_wtime()-start));

    //Calculate M
    double M = ((N*sumxy)-(sumx*sumy)) / ((N*sumxsq)-(sumx*sumx));
    //calculate C 
    double C = (sumy - (M*sumx)) / N;

	printf("The value of M is: %f\n",M);
	printf("The value of C is: %f\n",C);

}

//Initializes the value of Y according to specified algorithm from assignment question pow(10.0,-6)
double setY(int val) {
    return (1.0/exp(val/50000.1) + sin(val/501.1) / (log(val+1.0E-06)+cos(val/499.01+val)));
}
