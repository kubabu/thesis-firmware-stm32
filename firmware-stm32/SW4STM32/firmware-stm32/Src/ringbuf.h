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


ringbuf_t ringbuf(size_t capacity, float array[]);
ringbuf_t ringbuf3(size_t capacity, float array[], int items_count);

void ringbuf_push(ringbuf_t *buf, float val);

// check last added element to buffer
float ringbuf_peek(ringbuf_t *buf);

// only negative indices are valid!
// i = -1 is previous, -2 previous from previous and so on
float ringbuf_get_prev(ringbuf_t *buf, int16_t i);


// iterator size < buf->capacity
rbuf_iterator_t get_iterator(ringbuf_t *buf, size_t size);

float iterate(rbuf_iterator_t *iter, size_t i);


//void ringbufs_init(float **array, ringbuf_t *buffs, size_t buf_capacity, size_t buf_count);
//
//void iterators_init(ringbuf_t *buffs, size_t buf_count, size_t iter_size);


#endif /* INCLUDED_RINGBUF_H */
