// sorting.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <functional>
#include <algorithm>
#include <iostream>

#include <time.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>

void doQuickSort(int t);
void doMergeSort(int t);

void verify(unsigned short *list, int size)
{
	for (int i = 1; i < size; i++)
	{
		if (list[i] < list[i - 1])
		{
			std::cout << "FAILED" << std::endl;
			break;
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc != 3)	{
		printf("Sorting - Please provide the following arguments: \n  Algorithm (q = quicksort, m = mergesort) \n  Threshold value");
		return 1;
	}

	char* sort = argv[1];
	int threshold = atoi(argv[2]);

	if (strcmp(sort, "q") == 0)
	{
		doQuickSort(threshold);
	}
	else if (strcmp(sort, "m") == 0) {
		doMergeSort(threshold);
	}
	
	return 0;
}



