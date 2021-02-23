# ifndef __UFIFO_H_
# define __UFIFO_H_

#include <stdio.h>
#include <stdlib.h>

struct _kfifo {
	unsigned int in;
	unsigned int out;
	unsigned int mask;
	unsigned int esize;
	void         *data;
};

# endif
