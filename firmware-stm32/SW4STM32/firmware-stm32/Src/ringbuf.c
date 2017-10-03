#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <assert.h>

#include "ringbuf.h"


ringbuf_t ringbuf(size_t capacity, float array[]) {
	ringbuf_t result;

	result.capacity = capacity;
	result.buffer = array;
	result.head_index = -1;
	result.is_filled = 0;

	return result;
}

ringbuf_t ringbuf3(size_t capacity, float array[], int items_count) {
	ringbuf_t buf = ringbuf(capacity, array);

	if(items_count > 0) {
		buf.head_index = (items_count - 1) % capacity;
	} else {
		buf.head_index = -1;
	}
	return buf;
}


void ringbuf_push(ringbuf_t *buf, float val) {
	buf->head_index = (buf->head_index + 1) % buf->capacity;
	if(buf->head_index >= buf->capacity - 1) {
		buf->is_filled = 1;
	}
	buf->buffer[buf->head_index] = val;
}


float ringbuf_peek(ringbuf_t *buf) {
	if(buf == 0) {
		return 0.0;
	}
	return buf->buffer[buf->head_index];
}


float ringbuf_get_prev(ringbuf_t *buf, int16_t i) {
	if(buf == 0) {
		return 0.0;
	}
	int16_t j = buf->head_index + i;
	while(j < 0) {
		j += buf->capacity;
	}
	float val = buf->buffer[j];

	return val;
}


rbuf_iterator_t get_iterator(ringbuf_t *buf, size_t size) {
	rbuf_iterator_t result;
	result.buf = buf;
	result.size = size;

	return result;
}


float iterate(rbuf_iterator_t *iter, size_t i) {
	int16_t index = 1 - iter->size + i;
	float result = ringbuf_get_prev(iter->buf, index);

	return result;
}


//void ringbufs_init(float **array, ringbuf_t *buffs, size_t buf_capacity, size_t buf_count) {
//	for (int i = 0; i < buf_count; ++i) {
//		buffs[i] = ringbuf(buf_capacity, array[i]);
//	}
//}
//
//
//void iterators_init(ringbuf_t *buffs, size_t buf_count, size_t iter_size) {
//	;
//}
