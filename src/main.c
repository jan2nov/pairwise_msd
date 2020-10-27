#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "twoway_msd.h"
#include "textbook_msd.h"
#include <time.h>

#define max_width 5
#define n_repeat 10

//int main(int argc, char *argv[])
int main()
{
	printf("\n\t**********************************\n");
	double timer_start = 0.0;
	double timer_stop;

	unsigned int N = 4096;
	size_t data_size = N*sizeof(unsigned short);
	unsigned short *in_data; 

	timer_start = omp_get_wtime();
		in_data = (unsigned short*) malloc(N*sizeof(unsigned short));
	timer_stop = omp_get_wtime() - timer_start;
	printf("\t\tAllocation for data_size %lu of the memory: \t%*lf seconds\n", data_size, max_width, timer_stop);

	time_t t;
	srand((unsigned) time(&t));

	double control_mean;
	double control_sd;

	double test_mean;
	double test_sd;

	timer_start = omp_get_wtime();
	unsigned int myseed = omp_get_thread_num();
	#pragma omp parallel for
		for(unsigned int i = 0; i < N; i++){
//			in_data[i] = i % 256;
			in_data[i] = rand_r(&myseed) % 256;
		}
	timer_stop = omp_get_wtime() - timer_start;
	printf("\t\tPutting data to input:    \t%*lf seconds\n", max_width, timer_stop);

	////////////////////////////////////////////////////////////////////////////////////////////
	timer_start = omp_get_wtime();
		for (int i = 0; i < n_repeat; i++){
			printf("\t%d ... ", i);
			fflush(stdout);
			twoway_msd(N, in_data, &control_mean, &control_sd);
		}
	timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
	printf("\n\t\t----------------------------------\n");
	printf("\t\ttwo way bad algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
	fflush(stdout);
	////////////////////////////////////////////////////////////////////////////////////////////

//	timer_start = omp_get_wtime();
//	for (int i = 0; i < n_repeat; i++){
//			printf("\t%d ... ", i);		
//			fflush(stdout);
//			twoway_msd_parallel(N, in_data, &test_mean, &test_sd);
//	}
//	timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
//	printf("\n\t\t----------------------------------\n");
//	printf("\t\ttwo way bad parallel algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
//	fflush(stdout);
//	////////////////////////////////////////////////////////////////////////////////////////////
//	
//	timer_start = omp_get_wtime();
//	for (int i = 0; i < n_repeat; i++){
//			printf("\t%d ... ", i);		
//			fflush(stdout);
//			textbook_msd(N, in_data, &test_mean, &test_sd);
//	}
//	timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
//	printf("\n\t\t----------------------------------\n");
//	printf("\t\tTextbook algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
//	fflush(stdout);
//	////////////////////////////////////////////////////////////////////////////////////////////
	
        timer_start = omp_get_wtime();
	test_mean = 0;
        for (int i = 0; i < n_repeat; i++){
                        printf("\t%d ... ", i);
                        fflush(stdout);
                        textbook_msd_parallel(N, in_data, &test_mean, &test_sd);
        }
        timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
        printf("\n\t\t----------------------------------\n");
        printf("\t\tTextbook parallel algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
        fflush(stdout);

        timer_start = omp_get_wtime();
	test_mean = 0;
        for (int i = 0; i < n_repeat; i++){
                        printf("\t%d ... ", i);
                        fflush(stdout);
                        textbook_divide_msd_parallel(N, in_data, &test_mean, &test_sd);
        }
        timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
        printf("\n\t\t----------------------------------\n");
        printf("\t\tTextbook divide parallel algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
        fflush(stdout);
        ////////////////////////////////////////////////////////////////////////////////////////////
	printf("\n\t\t----------------------------------\n");
	printf("\t\tControl mean: %*lf\n\t\tControl std: %*lf\n", max_width, control_mean, max_width, control_sd);
	printf("\t\tTest mean: %*lf\n\t\tTest std: %*lf", max_width, test_mean, max_width, test_sd);
	printf("\n\t\t----------------------------------\n");

	free(in_data);

	// end of the code
	fflush(stdout); 
	printf("\n\t**********************************\n");
	printf( "\t        #That's all folks!        \n");
	printf( "\t**********************************\n\n");

	return 0;
}
