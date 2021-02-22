/* This file used to record some functions which are classcial. */
#include <stdint.h>
#include <stdbool.h>

static inline bool is_power_of_2(uint32_t n)
{
	/* eg. (1000 & 0111) == 0 */
	return (n != 0 && ((n & (n -1)) == 0));
}

//static inline uint32_t roundup_power_of_2(uint32_t a)
//{
//	if (a == 0)
//		return 0;
//
//	uint32_t position = 0;
//	for (int i = a; i != 0; i >>= 1)
//		position++;
//
//	return static_cast<uint32_t>(1 << position);
//}

static inline void swap(int *p, int *q)
{
	int temp = *p;
	*p = *q;
	*q = temp;
}
