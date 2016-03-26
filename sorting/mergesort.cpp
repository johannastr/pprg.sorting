#include "stdafx.h"

#include <functional>
#include <algorithm>
#include <iostream>
#include <random>

#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <time.h>
#include <math.h>

int thresholdM = 0;
unsigned short *output;

void merge2(unsigned short *list, int start, int mid, int end)
{
	int lengthLeft = mid - start + 1;
	int lengthRight = end - mid;

	unsigned short *leftArray = new unsigned short[lengthLeft + 1];
	unsigned short *rightArray = new unsigned short[lengthRight + 1];

	leftArray[0:lengthLeft] = list[start:lengthLeft];
	rightArray[0:lengthRight] = list[mid + 1:lengthRight];

	leftArray[lengthLeft] = 65535;
	rightArray[lengthRight] = 65535;

	int i = 0;
	int j = 0;
	for (int k = start; k < end; k++)
	{
		if (leftArray[i] <= rightArray[j])
		{
			list[k] = leftArray[i];
			i++;
		}
		else
		{
			list[k] = rightArray[j];
			j++;
		}
	}

	delete[] leftArray;
	delete[] rightArray;
}

void merge(unsigned short *list, int start, int mid, int end)
{
	int i = start;
	int j = mid;

	//std::cout << "start: " << start << ", mid: " << mid << ", end: " << end << std::endl;

	for (int k = start; k < end; k++) {
		//std::cout << "left: " << list[i] << ", right: " << list[j] << std::endl;
		if (i < mid && (j >= end || list[i] <= list[j])) {
			output[k] = list[i];
			i = i + 1;
		}
		else {
			output[k] = list[j];
			j = j + 1;
		}
		//std::cout << "index: " << k << ", value: " << output[k] << std::endl;
	}
}

void mergeSortSerial(unsigned short *list, int start, int end)
{
	if (start < end)
	{
		int mid = start + ((end - start) / 2);

		mergeSortSerial(list, start, mid);
		mergeSortSerial(list, mid + 1, end);

		merge(list, start, mid + 1, end + 1);
		list[start:end - start + 1] = output[start:end - start + 1];
	}
}

void mergeSortParallel(unsigned short *list, int start, int end)
{
	if (start < end)
	{
		int mid = start + ((end - start) / 2);

		if ((end - start) > thresholdM) {
			cilk_spawn mergeSortParallel(list, start, mid);
		}
		else {
			mergeSortParallel(list, start, mid);
		}
		mergeSortParallel(list, mid + 1, end);
		cilk_sync;

		merge(list, start, mid + 1, end + 1);
		list[start:end - start + 1] = output[start:end - start + 1];
	}
}

void verify(unsigned short *list, int size);

void doMergeSort(int t)
{
	thresholdM = t;

	std::uniform_int_distribution<> distribution(0, 65535);
	std::mt19937 engine;
	auto generator = std::bind(distribution, engine);

	int maxArraySize = pow(2, 26);
	int powerOfTwo = 19;

	for (int size = std::pow(2, powerOfTwo); size <= maxArraySize; size *= 2)
	{
		unsigned short *list = new unsigned short[size];
		output = new unsigned short[size];
		printf("Array size: 2^%d\n", powerOfTwo);

		/*------ SERIAL QUICKSORT ------*/
		std::generate_n(list, size, generator);

		clock_t start = clock();
		mergeSortSerial(list, 0, size - 1);
		clock_t end = clock();

		std::cout << "SERIAL: Merge Sort: " << end - start << " milliseconds" << std::endl;

		verify(output, size);

		/*------ PARALLEL QUICKSORT ------*/
		std::generate_n(list, size, generator);

		start = clock();
		mergeSortParallel(list, 0, size - 1);
		end = clock();

		std::cout << "PARALLEL: Merge Sort: " << end - start << " milliseconds" << std::endl;

		verify(output, size);

		delete[] list;
		delete[] output;

		printf("\n");
		powerOfTwo++;
	}
}