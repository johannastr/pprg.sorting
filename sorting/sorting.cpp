// sorting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// Cilk parallel program for Quick Sort Algorithm....
//#include <cilk.h>
//#include <cilkview.h>
#include <functional>
#include <algorithm>
#include <iostream>

#include <time.h>

void quick_sort(int *beg, int *end);

void quick_sort(int *beg, int *end) { 
	if (beg < end) { 
		end = end--; 
		int * pivot = std::partition(beg, end, std::bind2nd(std::less<int>(), *end));
		std::swap(*end, *pivot);
		//cilk_spawn quick_sort(beg, pivot);
		quick_sort(beg, pivot);
		pivot++; 
		end++; 
		quick_sort(pivot, end); 
		//cilk_sync; 
	} 
}

int main(int argc, char *argv[]) {
	int n; 
	std::cout << "Enter total number of elements that are to be sorted: ";
	std::cin >> n;
	int*  a = new int[n];
	//cilk_for(int x = 0; x < n; x++){ 
	for (int x = 0; x < n; x++){
		a[x] = x; 
	}
	std::random_shuffle(a, a + n); 

	std::cout << "Sorting " << n << " elements" << std::endl;
	/*for (int x = 0; x < n; x++) {
		std::cout << a[x] << ",";
	}
	std::cout << std::endl;*/
	std::cout << std::endl;

	clock_t start = clock(); // save start seconds
	//cilk::cilkview cilkview; 
	//cilkview.start(); 
	quick_sort(a, a + n); 
	//cilkview.stop(); 
	//cilkview.dump("Quick_Sort_Results", false); 
	clock_t end = clock();

	std::cout << "Quicksort on " << n << " items succeeded." << std::endl;
	/*for (int x = 0; x < n; x++) {
		if (x > 0) {
			std::cout << ",";
		}
		std::cout << a[x];
	}
	std::cout << std::endl;*/
	std::cout << std::endl;

	std::cout << "Total Time taken by Quick Sort: " << end - start << " milliseconds" << std::endl;

	std::cin >> n;
	return 0;
}



