/* 
 *@auther wanwenkai data:2021-02-07
 *ring_buffer.h
 **/

#ifndef KFIFO_HEADER_H
#define KFIFO_HEADER_H

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "tools.h"

struct ring_buffer {
	void	    *buffer;
	uint32_t	size;    //buffer size
	uint32_t	in;	
	uint32_t	out;
	//Lock use for multi producer and multi consumer
	pthread_mutex_t *lock;
};

struct ring_buffer *ring_buffer_init(void *buffer, uint32_t size, 
		pthread_mutex_t *lock)
{
	assert(buffer);
	struct ring_buffer *ring_buf = NULL;
	if (!is_power_of_2(size)) {
		fprintf(stderr, "size must be power of 2.\n");
		return NULL;
	}
	
	ring_buf = (struct int_buffer *)malloc(sizeof(struct ring_buffer));
	if (!ring_buf) {
		fprintf(stderr, "Failed to malloc memory, errno:%u, reason:%s", errno, strerror(errno));
	}
	/* reset 0 */
	memset(ring_buf, 0, sizeof(struct ring_buffer));
	/* init ring_buf */
	ring_buf->buffer = buffer;
	ring_buf->size = size;
	ring_buf->in = 0;
	ring_buf->out = 0;
	ring_buf->lock = lock;

	return ring_buf;
}


void ring_buffer_free(struct ring_buffer *ring_buf)
{
	if (!ring_buf)
		return;

	if (ring_buf->buffer) {
		free(ring_buf->buffer);
		ring_buf->buffer = NULL;
	}

	free(ring_buf);
	ring_buf = NULL;
}


uint32_t __ring_buffer_len(const struct ring_buffer *ring_buf)
{
	return (ring_buf->in - ring_buf->out);
}


uint32_t __ring_buffer_get(struct ring_buffer *ring_buf, void *buffer, uint32_t size)
{
	assert(ring_buf || buffer);

	uint32_t len = 0;
	size = min(size, ring_buf->in - ring_buf->out);
	len = min(size, ring_buf->size - (ring_buf->out & (ring_buf->size - 1)));
	memcpy(buffer, ring_buf->buffer + (ring_buf->out & (ring_buf->size - 1)), len);
	memcpy(buffer + len, ring_buf->buffer, size - len);
	ring_buf->out += size;

	return size;
}

uint32_t __ring_buffer_put(struct ring_buffer *ring_buf, void *buffer, uint32_t size)
{
	assert(ring_buf || buffer);
	uint32_t len = 0;

	size = min(size, ring_buf->size - ring_buf->in + ring_buf->out);
	len = min(size, ring_buf->size - (ring_buf->in & (ring_buf->size - 1)));
	memcpy(ring_buf->buffer + (ring_buf->in & (ring_buf->size - 1)), buffer, len);
	memcpy(ring_buf->buffer, buffer+len, size - len);
	ring_buf->in += size;

	return len;
}

uint32_t ring_buffer_get(struct ring_buffer *ring_buf, void *buffer, uint32_t size)
{
	uint32_t ret;

	pthread_mutex_lock(ring_buf->lock);
	ret = __ring_buffer_get(ring_buf, buffer, size);
	if (ring_buf->in == ring_buf->out)
		ring_buf->in = ring_buf->out = 0;
	pthread_mutex_unlock(ring_buf->lock);

	return ret;
}

uint32_t ring_buffer_put(struct ring_buffer *ring_buf, void *buffer, uint32_t size)
{
	uint32_t ret;

	pthread_mutex_lock(ring_buf->lock);
	ret = __ring_buffer_put(ring_buf, buffer, size);
	pthread_mutex_unlock(ring_buf->lock);

	return ret;
}

#endif
