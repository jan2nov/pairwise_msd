#include <math.h>
#include <stdio.h>

void textbook_msd(unsigned int N, unsigned short* in_data, double* mean, double* sd){
	double sum = 0.0;
	double temp_sd = 0.0;

	for(unsigned int i = 0; i < N; i++){
		register unsigned short temp;
		temp = in_data[i];
                temp_sd += temp*temp;
		sum += temp;
        }
	mean[0] = sum/N;
	sd[0] = temp_sd - (sum*sum)/N;
	sd[0] = sqrt(sd[0]/N);
}

void textbook_msd_parallel(unsigned int N, unsigned short* in_data, double* mean, double* sd){
	double sum = 0.0;
	double temp_sd = 0.0;
//	register unsigned short temp;

//	#pragma omp parallel for reduction (+: temp_sd, sum) private(temp) schedule(auto)
	#pragma omp parallel for reduction (+: temp_sd, sum) schedule(auto)
	for(unsigned int i = 0; i < N; i++){
//		temp = in_data[i];
//              temp_sd += temp*temp;
//		sum += temp;
		temp_sd += in_data[i]*in_data[i];
		sum += in_data[i];
        }
	mean[0] = sum/N;
	sd[0] = temp_sd - (sum*sum)/N;
	sd[0] = sqrt(sd[0]/N);
}


void textbook_divide_msd_parallel(unsigned int N, unsigned short* in_data, double* mean, double* sd){

	int num_block = 8;
        double sum[num_block];
        double temp_sd[num_block];

	int block_length = N/num_block;
	int block_remainder = N - num_block*block_length;
	double rest_sum = 0.0;
	double rest_sd = 0.0;

	for(int i = 0; i < num_block; i++){
		sum[i] = 0.0;
		temp_sd[i] = 0.0;
	}

	for(int i = 0; i < num_block; i++){
		int shift = i*block_length;
		#pragma omp parallel for reduction (+: temp_sd, sum)
		for(int j = 0; j < block_length; j++){
	                temp_sd[i] += in_data[j + shift]*in_data[j + shift];
	                sum[i] += in_data[j + shift];
	        }
		temp_sd[i] = temp_sd[i] - sum[i]*sum[i]/block_length;
	}
	if (block_remainder != 0){
		for(int i = 0; i < block_remainder; i++){
			rest_sum += in_data[num_block*block_length + i];
			rest_sd += in_data[num_block*block_length + i]*in_data[num_block*block_length + i];
		}
	}

	double final_sum = 0;
	#pragma omp parallel for reduction (+: final_sum)
	for(int i = 0; i < num_block; i++){
		final_sum += sum[i];
//		printf("%d %lf\n", i*block_length,final_sum);
	}

	int step = num_block;
	int inner_step = 1;
	int stop = log2(num_block);
	for (int i = 0; i < stop; i++){
		step = step/2;
		for (int j = 0; j < step; j++){
//			temp_sd[j*2*inner_step] += temp_sd[2*j*inner_step + inner_step] + (1/(2*num_block))*(2*num_block*sum[j*2*inner_step] - sum[2*j*inner_step + inner_step])*(2*num_block*sum[j*2*inner_step] - sum[2*j*inner_step + inner_step]);
			temp_sd[2*j*inner_step] += temp_sd[2*j*inner_step + inner_step] + (1.0/(2*block_length))*(sum[2*j*inner_step] - sum[2*j*inner_step + inner_step])*(sum[2*j*inner_step] - sum[2*j*inner_step + inner_step]);
		}
		inner_step = inner_step*2;
	}
	if (block_remainder != 0){
		temp_sd[0] += rest_sd + ((block_length)/(block_remainder*(block_length + block_remainder)))*((block_remainder + block_length)/(block_length)*final_sum - rest_sum)*((block_remainder + block_length)/(block_length)*final_sum - rest_sum);
		final_sum += rest_sum;
	}

        mean[0] = final_sum/N;
        sd[0] = sqrt(temp_sd[0]/N);
}

