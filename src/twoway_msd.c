#include <math.h>
#include <stdio.h>

void twoway_msd(unsigned int first, unsigned int second, unsigned short* in_data, double* mean, double* sd){
	double sum;
	double temp_sd;

	for(unsigned int i = 0; i < first; i++){
		sum = 0.0;
		temp_sd = 0.0;
		for(unsigned int j = 0; j < second; j++){
			sum += in_data[i*second + j];
		}
		sum = sum/second;
		for(unsigned int j = 0; j < second; j++){
                        temp_sd += (in_data[i*second + j] - sum)*(in_data[i*second + j] - sum);
                }
		mean[i] = sum;
		sd[i] = sqrt(temp_sd/second);
	}
}


void twoway_msd_parallel(unsigned int first, unsigned int second, unsigned short* in_data, double* mean, double* sd){
	double sum = 0.0;
	double temp_sd = 0.0;

	#pragma omp parallel for reduction(+:sum) schedule(auto)
	for(unsigned int i = 0; i < first; i++){
		sum = 0.0;
		for(unsigned int j = 0; j < second; j++){
			sum += in_data[i*second + j];
		}
		mean[i] = sum/second;
	}

	#pragma omp parallel for reduction(+:temp_sd) schedule(auto)
	for(unsigned int i = 0; i < first; i++){
		temp_sd = 0;
		for(unsigned int j = 0; j < second; j++){
			temp_sd += (in_data[i*second + j] - mean[i])*(in_data[i*second + j] - mean[i]);
		}
		sd[i] = sqrt(temp_sd/second);
	}

}
