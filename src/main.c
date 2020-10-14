#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "twoway_msd.h"

#define max_width 5

//int main(int argc, char *argv[])
int main()
{
	printf("\n\t**********************************\n");
	double timer_start = 0.0;
	double timer_stop;

	unsigned int N = 1024*1024*1024;
	unsigned short *in_data; 

	timer_start = omp_get_wtime();
		in_data = (unsigned short*) malloc(N*sizeof(unsigned short));
	timer_stop = omp_get_wtime() - timer_start;
	printf("\t\tAllocation of the memory: \t%*lf seconds\n", max_width, timer_stop);

	double control_mean;
	double control_sd;

	timer_start = omp_get_wtime();
	#pragma omp parallel for
		for(unsigned int i = 0; i < N; i++){
			in_data[i] = i % 255;
		}
	timer_stop = omp_get_wtime() - timer_start;
	printf("\t\tPutting data to input:    \t%*lf seconds\n", max_width, timer_stop);

	timer_start = omp_get_wtime();
		twoway_msd(N, in_data, &control_mean, &control_sd);
	timer_stop = omp_get_wtime() - timer_start;
	printf("\n\t\t----------------------------------\n");
	printf("\t\tTwo way bad algorithm:    \t%*lf seconds.\n\t\tBandwidth: \t%*lf GB/s.", max_width, timer_stop, max_width, (N*sizeof(unsigned short)/timer_stop)/1000000000);


	printf("\n\t\t----------------------------------\n");
	printf("\t\tControl mean: %*lf\n\t\tControl std: %*lf", max_width, control_mean, max_width, control_sd);
	printf("\n\t\t----------------------------------\n");

	free(in_data);

	// end of the code
	fflush(stdout); 
	printf("\n\t**********************************\n");
	printf( "\t        #That's all folks!        \n");
	printf( "\t**********************************\n\n");

	return 0;
}
