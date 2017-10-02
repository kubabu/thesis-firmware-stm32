#ifndef INCLUDED_RINGBUF_H
#define INCLUDED_RINGBUF_H

/*
 * ringbuf.h - C ring buffer (FIFO) interface.
 */

#include <stddef.h>
#include <sys/types.h>



typedef struct ringbuf_t {
	float *buffer;
	int16_t head_index;
    size_t capacity;
    uint8_t is_filled;
} ringbuf_t;



// structure allowing to iterate over last n elements in cyclic buffer
typedef struct rbuf_iterator_t {
	ringbuf_t *buf;
	size_t size;
} rbuf_iterator_t;


ringbuf_t ringbuf_init(size_t capacity, float array[]);

void ringbuf_push(ringbuf_t *buf, float val);

// check last added element to buffer
float ringbuf_peek(ringbuf_t *buf);

// only negative indices are valid!
// i = -1 is previous, -2 previous from previous and so on
float ringbuf_get_prev(ringbuf_t *buf, int16_t i);


// iterator size < buf->capacity
rbuf_iterator_t get_iterator(ringbuf_t *buf, size_t size);

rbuf_iterator_t get_iterator3(float *array, size_t buffer_capacity, size_t iterator_size);

float iterate(rbuf_iterator_t iter, size_t i);


#endif /* INCLUDED_RINGBUF_H */
