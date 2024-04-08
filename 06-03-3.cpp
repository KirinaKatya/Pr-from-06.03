#include <iostream>
#include <random>
#include <omp.h>
#include <thread>

using namespace std;

int computePi(int numPoints, int numThreads) {
    int numInsideCircle = 0;

#pragma omp parallel num_threads(numThreads) reduction(+:numInsideCircle)
    {
        unsigned int seed = omp_get_thread_num();
        mt19937 rng(seed);
        uniform_real_distribution<double> dist(0, 1);

#pragma omp for
        for (int i = 0; i < numPoints; i++) {
            double x = dist(rng);
            double y = dist(rng);
            if (x * x + y * y <= 1.0 * 1.0) {
                numInsideCircle++;
            }
        }
    }

    return numInsideCircle;
}

int main() {
    int numPoints = 10000000;
    int numThreads = thread::hardware_concurrency();
    cout << "Count of threads: " << numThreads << endl;
    cout << "Count of points: " << numPoints << endl << endl;

    double startTime = omp_get_wtime();
    int numInsideCircle = computePi(numPoints, numThreads);
    double endTime = omp_get_wtime();

    double pi = 4.0 * numInsideCircle / numPoints;
    cout << "Value of pi with " << numThreads << " threads: " << pi << endl;
    cout << "Time with " << numThreads << " threads: " << endTime - startTime << " seconds" << endl;

    numThreads = 4;
    startTime = omp_get_wtime();
    numInsideCircle = computePi(numPoints, numThreads);
    endTime = omp_get_wtime();

    pi = 4.0 * numInsideCircle / numPoints;
    cout << "\nValue of pi with 4 threads: " << pi << endl;
    cout << "Time with 4 threads: " << endTime - startTime << " seconds" << endl;

    return 0;
}
