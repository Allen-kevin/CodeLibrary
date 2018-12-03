#include "algorithm.h"

int* maxsubarray::FindMaxCrossingSubArray(int *array, int low, int mid, int high)
{
	int left_sum = INT_MIN;
	int right_sum = INT_MIN;
	int sum = 0;
	int max_left = 0, max_right = 0;
	int* temp = new int[3];

	for (int i = mid; i >= low; i--) {
		sum = sum + array[i];
		if (sum > left_sum) {
			left_sum = sum;
			max_left = i;
		}
	}

	sum = 0;
	for (int j = mid+1; j <= high; j++) {
		sum = sum + array[j];	
		if (sum > right_sum) {
			right_sum = sum;
			max_right = j;
		}
	}
	
	temp[0] = max_left;
	temp[1] = max_right;
	temp[2] = left_sum + right_sum;

	return temp;
}

/* time complexity is O(nlgn) */
int* maxsubarray::FindMaximumSubArray(int *array, int low, int high)
{
	int* temp = new int[3];
	if (high == low) {
		temp[0] = low;
		temp[1] = high;
		temp[2] = array[low];
		return temp;
	} else {
		int mid = (low + high)/2;
		//temp_left = [left_low, left_high, left_sum]
		int* temp_left = FindMaximumSubArray(array, low, mid);
		//temp_right = [right_low, right_high, right_sum]
		int* temp_right = FindMaximumSubArray(array, mid+1, high);
		//temp_corss = [cross_low, cross_high, cross_sum] 
		int* temp_cross = FindMaxCrossingSubArray(array, low, mid, high);

		if (temp_left[2] >= temp_right[2] && temp_left[2] >= temp_cross[2])
			return temp_left;
		else if (temp_right[2] >= temp_left[2] && temp_right[2] >= temp_cross[2])
			return temp_right;
		else
			return temp_cross;
	}
}

/* time complexity is O(n) */
int maxsubarray::MaximumSubArray_Linear(int *array, int low, int high)
{
	int max_sum = array[low];
	int sum = 0;

	for (int i = low; i <= high; i++) {
		sum += array[i];
		if (sum > max_sum)
			max_sum = sum;
		if (sum <= 0)
			sum = 0;
	}

	return max_sum;
}
