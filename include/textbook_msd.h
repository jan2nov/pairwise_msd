#include <stdbool.h>

void textbook_msd(unsigned int first, unsigned int second, unsigned short* in_data, double* mean, double* sd);
void textbook_msd_parallel(unsigned int first, unsigned int second, unsigned short* in_data, double* mean, double* sd, bool outlier_rejection, double old_mean, double old_stdev, double sigma);
void textbook_divide_msd_parallel(unsigned int N, unsigned short* in_data, double* mean, double* sd);
