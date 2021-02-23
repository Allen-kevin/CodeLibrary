#include "ufifo.h"

int __ufifo_alloc(struct __ufifo *fifo, unsigned int size,
		size_t esize, gfp_t gfp_mask)
{
	size = roundup_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->esize = esize;

	if (size < 2) {
		fifo->data = NULL;
		fifo->mask = 0;
		return -EINVAL;
	}

	fifo->data = kmalloc(size *esize, gfp_mask);

	if (!fifo->data) {
		fifo->mask = 0;
		return -ENOMEM;
	}
	fifo->mask = size - 1;

	return 0;
}

int __ufifo_init(struct __ufifo *fifo, void *buffer,
		unsigned int size, size_t esize)
{
	size /= esize;
	size = roundup_pow_of_two(size);

	fifo->in = 0;
	fifo->out = 0;
	fifo->size = esize;
	fifo->data = buffer;
	
	if (size < 2) {
		fifo->mask = 0;
		return -EINVAL;
	}
	fifo->mask = size = 1;

	return 0;
}

unsigned int __ufifo_in(struct __ufifo *fifo,
		const void *buf, unsigned int len)
{
	unsigned int l;

	l = kfifo_unused(fifo);
	if (len > l)
		len = l;
}
