// sorting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <functional>
#include <algorithm>
#include <iostream>
#include <random>

#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>

int thresholdQ = 0;

void quickSortParallel(unsigned short *beg, unsigned short *end) {
	if (beg < end) {
		end = end--;
		unsigned short *pivot = std::partition(beg, end, std::bind2nd(std::less<unsigned short>(), *end));
		std::swap(*end, *pivot);
		if ((end - beg) > thresholdQ) {
			cilk_spawn quickSortParallel(beg, pivot);
		}
		else {
			quickSortParallel(beg, pivot);
		}
		pivot++;
		end++;
		quickSortParallel(pivot, end);
		cilk_sync; 
	}
}

void quickSortSerial(unsigned short *beg, unsigned short *end) {
	if (beg < end) {
		end = end--;
		unsigned short *pivot = std::partition(beg, end, std::bind2nd(std::less<unsigned short>(), *end));
		std::swap(*end, *pivot);
		quickSortSerial(beg, pivot);
		pivot++;
		end++;
		quickSortSerial(pivot, end);
	}
}

void verify(unsigned short *list, int size);

void doQuickSort(int t) {
	thresholdQ = t;

	std::uniform_int_distribution<> distribution(0, 65535);
	std::mt19937 engine;
	auto generator = std::bind(distribution, engine);

	int maxArraySize = pow(2, 23);
	int powerOfTwo = 17;

	for (int size = std::pow(2, powerOfTwo); size <= maxArraySize; size *= 2)
	{
		unsigned short *list = new unsigned short[size];
		printf("Array size: 2^%d\n", powerOfTwo);

		/*------ SERIAL QUICKSORT ------*/
		std::generate_n(list, size, generator);

		clock_t start = clock();
		quickSortSerial(list, list + size);
		clock_t end = clock();

		std::cout << "SERIAL: Quick Sort: " << end - start << " milliseconds" << std::endl;
		
		verify(list, size);

		/*------ PARALLEL QUICKSORT ------*/
		std::generate_n(list, size, generator);

		start = clock();
		quickSortParallel(list, list + size);
		end = clock();

		std::cout << "PARALLEL: Quick Sort: " << end - start << " milliseconds" << std::endl;

		verify(list, size);

		delete[] list;

		printf("\n");
		powerOfTwo++;
	}	
}



