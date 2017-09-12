#include <stdio.h>

void merge(double *list, int left_start, int left_end, int right_start, int right_end)
{
	/* calculate temporary list sizes */
	int left_length = left_end - left_start;
	int right_length = right_end - right_start;

#ifdef _WIN32
    double * left_half = new double [left_length];
    double * right_half = new double [right_length];
#elif defined _WIN64
    double * left_half = new double [left_length];
    double * right_half = new double [right_length];
#elif defined __unix__
    double left_half[left_length];
	double right_half[right_length];
#elif defined __APPLE__
    double left_half[left_length];
    double right_half[right_length];
#else
    printf("Unsupported operating system\n");
#endif

	int r = 0; /* right_half index */
	int l = 0; /* left_half index */
	int i = 0; /* list index */

	/* copy left half of list into left_half */
	for (i = left_start; i < left_end; i++, l++)
	{
		left_half[l] = list[i];
	}

	/* copy right half of list into right_half */
	for (i = right_start; i < right_end; i++, r++)
	{
		right_half[r] = list[i];
	}

	/* merge left_half and right_half back into list */
	for ( i = left_start, r = 0, l = 0; l < left_length && r < right_length; i++)
	{
		if ( left_half[l] < right_half[r] ) { list[i] = left_half[l++]; }
		else { list[i] = right_half[r++]; }
	}

	/* Copy over leftovers of whichever temporary list hasn't finished */
	for ( ; l < left_length; i++, l++) { list[i] = left_half[l]; }
	for ( ; r < right_length; i++, r++) { list[i] = right_half[r]; }
}

void mergesort_r(int left, int right, double *list)
{
	/* Base case, the list can be no simpiler */
	if (right - left <= 1)
	{
		return;
	}

	/* set up bounds to slice array into */
	int left_start  = left;
	int left_end    = (left+right)/2;
	int right_start = left_end;
	int right_end   = right;

	/* sort left half */
	mergesort_r( left_start, left_end, list);
	/* sort right half */
	mergesort_r( right_start, right_end, list);

	/* merge sorted halves back together */
	merge(list, left_start, left_end, right_start, right_end);
}

void mergesort(double *list, int length)
{
	mergesort_r(0, length, list);
}

void print_list(double *list, int len)
{
	int i;

	printf("[ ");
	for (i = 0; i < len; i++)
	{
		printf("%.10f ", list[i]);
	}
	printf("]\n");
}








