#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* leftArr = new int[n1];
    int* rightArr = new int[n2];

    for (int i = 0; i < n1; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        rightArr[j] = arr[mid + 1 + j];
    int i = 0, j = 0;
    int k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
    delete[] leftArr;
    delete[] rightArr;
}

void sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        sort(arr, left, mid);
        sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void sortParallel(int arr[], int left, int right, int countOfThreads) {
    if (left < right) {
        int mid = left + (right - left) / 2;

#pragma omp parallel sections num_threads(countOfThreads)
        {
#pragma omp section
            sortParallel(arr, left, mid, countOfThreads);

#pragma omp section
            sortParallel(arr, mid + 1, right, countOfThreads);
        }

#pragma omp critical
        merge(arr, left, mid, right);
    }
}

int main() {
    const int N = 1000;
    int arr[N];
    srand(time(0));
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 100;
    }
    const int countOfThreads = thread::hardware_concurrency();
    cout << "Number of threads: " << countOfThreads << endl;

    auto start = chrono::high_resolution_clock::now();
    sort(arr, 0, N - 1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "One thread sorting time: " << duration.count() << " seconds" << endl;

    start = chrono::high_resolution_clock::now();
    sortParallel(arr, 0, N - 1, countOfThreads);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "16 threads sorting time: " << duration.count() << " seconds" << endl;
    cout << "_____________________________________________" << endl;
    cout << "Sorted Array: ";
    for (int i = 0; i < N; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
