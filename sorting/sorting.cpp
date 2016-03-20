// sorting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <functional>
#include <algorithm>
#include <iostream>

#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

void quick_sort_parallel(int *beg, int *end) {
	if (beg < end) {
		end = end--;
		int *pivot = std::partition(beg, end, std::bind2nd(std::less<int>(), *end));
		std::swap(*end, *pivot);
		cilk_spawn quick_sort_parallel(beg, pivot);
		pivot++;
		end++;
		quick_sort_parallel(pivot, end);
		cilk_sync; 
	}
}

void quick_sort_serial(int *beg, int *end) {
	if (beg < end) {
		end = end--;
		int *pivot = std::partition(beg, end, std::bind2nd(std::less<int>(), *end));
		std::swap(*end, *pivot);
		quick_sort_serial(beg, pivot);
		pivot++;
		end++;
		quick_sort_serial(pivot, end);
	}
}

int main(int argc, char *argv[]) {
	int n;
	std::cout << "Enter total number of elements to be sorted: ";
	std::cin >> n;

	int* a = new int[n];
	int* b = new int[n];
	cilk_for(int x = 0; x < n; x++){ 
	//for (int x = 0; x < n; x++){
		a[x] = x; 
	}

	std::random_shuffle(a, a + n);
	std::copy(a, a + n, b);

	/*------ SERIAL QUICKSORT ------*/

	std::cout << "SERIAL: Sorting " << n << " elements (First: " << *a << ", Last: " << *(a + n -1) << ")" << std::endl;
	/*for (int x = 0; x < n; x++) {
		std::cout << a[x] << ",";
	}
	std::cout << std::endl;*/

	clock_t start_s = clock();
	quick_sort_serial(a, a + n);
	clock_t end_s = clock();

	std::cout << "SERIAL: Quicksort on " << n << " items succeeded. (First: " << *a << ", Last: " << *(a + n - 1) << ")" << std::endl;
	/*for (int x = 0; x < n; x++) {
		if (x > 0) {
			std::cout << ",";
		}
		std::cout << a[x];
	}
	std::cout << std::endl;*/

	std::cout << "SERIAL: Total Time taken by Quick Sort: " << end_s - start_s << " milliseconds" << std::endl;

	/*------ PARALLEL QUICKSORT ------*/
	
	std::cout << "PARALLEL: Sorting " << n << " elements (First: " << *b << ", Last: " << *(b + n - 1) << ")" << std::endl;

	clock_t start_p = clock();
	quick_sort_parallel(b, b + n);
	clock_t end_p = clock();

	std::cout << "PARALLEL: Quicksort on " << n << " items succeeded. (First: " << *b << ", Last: " << *(b + n - 1) << ")" << std::endl;
	std::cout << "PARALLEL: Total Time taken by Quick Sort: " << end_p - start_p << " milliseconds" << std::endl;
	
	std::cin >> n;
	return 0;
}



