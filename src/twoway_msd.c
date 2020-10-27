#include <math.h>
#include <stdio.h>

void twoway_msd(unsigned int N, unsigned short* in_data, double* mean, double* sd){
	double sum = 0.0;
//	int block = N/8;
	for(unsigned int i = 0; i < N; i++){
		sum += in_data[i];
//		if (i % block == 0) printf("\n%d %lf",i, sum);
	}
	mean[0] = sum/N;
	sum = 0.0;
	for(unsigned int i = 0; i < N; i++){
		sum += (in_data[i] - mean[0])*(in_data[i] - mean[0]);
	}
	sd[0] = sqrt(sum/N);
}


void twoway_msd_parallel(unsigned int N, unsigned short* in_data, double* mean, double* sd){
	double sum = 0.0;
	#pragma omp parallel for reduction(+:sum) schedule(auto)
	for(unsigned int i = 0; i < N; i++){
		sum += in_data[i];
	}
	mean[0] = sum/N;
	sum = 0.0;
	#pragma omp parallel for reduction(+:sum) schedule(auto)
	for(unsigned int i = 0; i < N; i++){
		sum += (in_data[i] - mean[0])*(in_data[0] - mean[0]);
	}
	sd[0] = sqrt(sum/N);
}
