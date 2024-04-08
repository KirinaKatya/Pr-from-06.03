#include <iostream>
#include "omp.h"
#include <thread>

using namespace std;

int main()
{
	const int N = 1000;
	int arr[N];
	for (int i = 0; i < N; i++) {
		arr[i] = rand() % 100;
	}
	const int countOfThreads = thread::hardware_concurrency();  
	cout << "Count of threads: " << countOfThreads << endl;

	float totalSum = 0, totalSum1 = 0;
	for (int i = 0; i < N; i++) {
		totalSum1 += arr[i];
	}
	cout << "Total sum without threads: " << totalSum1 << endl;
#pragma omp parallel reduction(+:totalSum) num_threads(countOfThreads)
	{
#pragma omp for
		for (int i = 0; i < 1000; i++) {
			totalSum += arr[i];
		}
	}
	cout << "Total sum with 16 threads: " << totalSum << endl;

	return 0;
}