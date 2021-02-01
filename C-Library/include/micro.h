#ifndef __MICRO_H
#define __MICRO_H

#define MALLOC(n, type) \
	((type*)malloc((n)*sizeof(type)))

#endif
