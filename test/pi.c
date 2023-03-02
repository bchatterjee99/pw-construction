#include <stdio.h>
#include <omp.h>


static long num_steps = 100000;
double step;

#define THREADS 4

int main()
{
    int i; double pi,sum = 0;
    step = 1.0/(double)num_steps;

    double tsum[THREADS];

    #pragma omp parallel
    {
	int num = omp_get_num_threads();
	int id = omp_get_thread_num();
	int block = num_steps / num;
	double x;
	tsum[id] = 0;

	printf("thread %d range %d - %d\n", id, id * block, (id + 1) * block);
	for(int i = id * block; i < (id+1) * block; i++)
	{
	    x = (i + 0.5) * step;
	    tsum[id] = tsum[id] + 4.0/(1.0 + x*x);
	}
    }

    for(int i=0; i<THREADS; i++)
    {
	printf("thread %d sum: %lf\n", i, tsum[i]);
	sum = sum + tsum[i];
    }
    printf("sum = %lf\n", sum);
    pi = sum * step;

    printf("pi = %lf\n\n", pi);

    return 0;
}
