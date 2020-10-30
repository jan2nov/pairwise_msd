void result_control(unsigned int tsamp , double *control_mean, double *control_sd, double *test_mean, double *test_sd, double *result_mean, double *result_sd){
	double temp_mean = 0;
	double temp_sd = 0;

	#pragma omp parallel for reduction (+: temp_mean, temp_sd)
	for(unsigned int i = 0; i < tsamp; i++){
		temp_mean += (control_mean[i] - test_mean[i])*(control_mean[i] - test_mean[i]);
		temp_sd += (control_sd[i] - test_sd[i])*(control_sd[i] - test_sd[i]);
	}

	result_mean[0] = temp_mean;
	result_sd[0] = temp_sd;

}
