#include <math.h>

void twoway_msd(unsigned int N, unsigned short* in_data, double* mean, double* sd){
	double sum = 0.0;
	for(unsigned int i = 0; i < N; i++){
		sum += in_data[i];
	}
	mean[0] = sum/N;
	sum = 0.0;
	for(unsigned int i = 0; i < N; i++){
		sum += (in_data[i] - mean[0])*(in_data[i] - mean[0]);
	}
	sd[0] = sqrt(sum/N);
}
