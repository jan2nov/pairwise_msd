#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "twoway_msd.h"
#include "textbook_msd.h"
#include "result_control.h"
#include <time.h>

#define max_width 5
#define n_repeat 10

//int main(int argc, char *argv[])
int main()
{
	printf("\n\t**********************************\n");
	double timer_start = 0.0;
	double timer_stop;

	unsigned int nchans = 4096;
	unsigned int tsamp = 1000000;
	size_t N = nchans*tsamp;
	size_t data_size = N*sizeof(unsigned short);
	unsigned short *in_data; 

	timer_start = omp_get_wtime();
		in_data = (unsigned short*) malloc(N*sizeof(unsigned short));
	timer_stop = omp_get_wtime() - timer_start;
	printf("\t\tAllocation for data_size %lu of the memory: \t%*lf seconds\n", data_size, max_width, timer_stop);

	double *control_mean;
	double *control_sd;

	double *test_mean;
	double *test_sd;

	double result_mean = 0.0;
        double result_sd = 0.0;

	control_mean = (double*) malloc(tsamp*sizeof(double));
	control_sd = (double*) malloc(tsamp*sizeof(double));
	test_mean = (double*) malloc(tsamp*sizeof(double));
	test_sd = (double*) malloc(tsamp*sizeof(double));

	timer_start = omp_get_wtime();
//	unsigned int myseed = omp_get_thread_num();
	#pragma omp parallel for
	for(unsigned int i = 0; i < (unsigned int)N; i++){
		in_data[i] = i % 4096;
//			in_data[i] = rand_r(&myseed) % 4096;
	}

	#pragma omp parallel for
	for(unsigned int i = 0; i < tsamp; i++){
		control_mean[i] = 0.0f;
		control_sd[i] = 0.0f;
		test_mean[i] = 0.0f;
		test_sd[i] = 0.0f;
	}
	timer_stop = omp_get_wtime() - timer_start;
	printf("\t\tPutting data to input:    \t%*lf seconds\n", max_width, timer_stop);

	////////////////////////////////////////////////////////////////////////////////////////////
	timer_start = omp_get_wtime();
			fflush(stdout);
			twoway_msd(tsamp, nchans, in_data, control_mean, control_sd);
	timer_stop = (omp_get_wtime() - timer_start);
	printf("\n\t\t----------------------------------\n");
	printf("\t\ttwo way bad algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
	fflush(stdout);
	////////////////////////////////////////////////////////////////////////////////////////////

	timer_start = omp_get_wtime();
	for (int i = 0; i < n_repeat; i++){
			printf("\t%d ... ", i);		
			fflush(stdout);
			twoway_msd_parallel(tsamp, nchans, in_data, test_mean, test_sd);
	}
	timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
	printf("\n\t\t----------------------------------\n");
	printf("\t\ttwo way bad parallel algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
	result_control(tsamp , control_mean, control_sd, test_mean, test_sd, &result_mean, &result_sd);
	printf("\t\tResult mean: %*lf\n\t\tResult std: %*lf\n", max_width, result_mean, max_width, result_sd);
	fflush(stdout);
	////////////////////////////////////////////////////////////////////////////////////////////
	
//	timer_start = omp_get_wtime();
//	for (int i = 0; i < n_repeat; i++){
//			printf("\t%d ... ", i);		
//			fflush(stdout);
//			textbook_msd(tsamp, nchans, in_data, test_mean, test_sd);
//	}
//	timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
//	printf("\n\t\t----------------------------------\n");
//	printf("\t\tTextbook algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
//      result_control(tsamp , control_mean, control_sd, test_mean, test_sd, &result_mean, &result_sd);
//      printf("\t\tResult mean: %*lf\n\t\tResult std: %*lf\n", max_width, result_mean, max_width, result_sd);
//	fflush(stdout);
	////////////////////////////////////////////////////////////////////////////////////////////
	
        timer_start = omp_get_wtime();
        for (int i = 0; i < n_repeat; i++){
                        printf("\t%d ... ", i);
                        fflush(stdout);
                        textbook_msd_parallel(tsamp, nchans, in_data, test_mean, test_sd, false, 0, 1, 1);
        }
        timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
        printf("\n\t\t----------------------------------\n");
        printf("\t\tTextbook parallel algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
        result_control(tsamp , control_mean, control_sd, test_mean, test_sd, &result_mean, &result_sd);
        printf("\t\tResult mean: %*lf\n\t\tResult std: %*lf\n", max_width, result_mean, max_width, result_sd);
        fflush(stdout);
	////////////////////////////////////////////////////////////////////////////////////////////
	
//        timer_start = omp_get_wtime();
//        for (int i = 0; i < n_repeat; i++){
//                        printf("\t%d ... ", i);
//                        fflush(stdout);
//                        textbook_divide_msd_parallel(N, in_data, &test_mean, &test_sd);
//        }
//        timer_stop = (omp_get_wtime() - timer_start)/n_repeat;
//        printf("\n\t\t----------------------------------\n");
//        printf("\t\tTextbook divide parallel algorithm:    \t%*lf seconds.\n\t\tbandwidth: \t%*lf GB/s.\n\n", max_width, timer_stop, max_width, data_size/timer_stop/1e9);
//        fflush(stdout);
        ////////////////////////////////////////////////////////////////////////////////////////////
	
	result_control(tsamp , control_mean, control_sd, test_mean, test_sd, &result_mean, &result_sd);
	
	printf("\n\t\t----------------------------------\n");
	printf("\t\tControl mean: %*lf\n\t\tControl std: %*lf\n", max_width, control_mean[1], max_width, control_sd[1]);
	printf("\t\tResult mean: %*lf\n\t\tResult std: %*lf\n", max_width, result_mean, max_width, result_sd);
	printf("\t\tTest mean: %*lf\n\t\tTest std: %*lf", max_width, test_mean[1], max_width, test_sd[1]);
	printf("\n\t\t----------------------------------\n");

	free(in_data);
	free(control_mean);
	free(control_sd);
	free(test_mean);
	free(test_sd);

	// end of the code
	fflush(stdout); 
	printf("\n\t**********************************\n");
	printf( "\t        #That's all folks!        \n");
	printf( "\t**********************************\n\n");

	return 0;
}
