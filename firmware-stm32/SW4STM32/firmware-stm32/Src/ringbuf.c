#include "ringbuf.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <assert.h>


ringbuf_t ringbuf_init(size_t capacity, float array[]) {
	ringbuf_t result;

	result.capacity = capacity;
	result.buffer = array;
	result.head_index = -1;
	result.is_filled = 0;

	return result;
}


void ringbuf_push(ringbuf_t *buf, float val) {
	buf->head_index = (buf->head_index + 1) % buf->capacity;
	if(buf->head_index >= buf->capacity - 1) {
		buf->is_filled = 1;
	}
	buf->buffer[buf->head_index] = val;
}


float ringbuf_peek(ringbuf_t *buf) {
	return buf->buffer[buf->head_index];
}


float ringbuf_get_prev(ringbuf_t *buf, int16_t i) {
	int16_t j = buf->head_index + i;

	while(j < 0) {
		j += buf->capacity;
	}

	return buf->buffer[j];
}


rbuf_iterator_t get_iterator(ringbuf_t *buf, size_t size) {
	rbuf_iterator_t result;
	result.buf = buf;
	result.size = size;

	return result;
}


rbuf_iterator_t get_iterator3(float *array, size_t buf_capacity, size_t iter_size) {
	ringbuf_t buf = ringbuf_init(buf_capacity, array);

	return get_iterator(&buf, iter_size);
}


float iterate(rbuf_iterator_t *iter, size_t i) {
	int16_t index = 1 - iter->size + i;

	return ringbuf_get_prev(iter->buf, index);
}

