#include "ringbuf.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <assert.h>



ringbuf_t init(size_t capacity, float buffer[]) {
	ringbuf_t result;

	result.capacity = capacity;
	result.buffer = buffer;
	result.head_index = 0;
	result.is_filled = 0;

	return result;
}


void push(ringbuf_t *buf, float val) {
	buf->buffer[buf->head_index] = val;
	buf->head_index++;
	if(buf->head_index >= buf->capacity) {
		buf->head_index = 0;
		buf->is_filled = 1;
	}
}


//
//ringbuf_t ringbuf_new(size_t capacity)
//{
//    ringbuf_t rb = malloc(sizeof(struct ringbuf_t));
//    if (rb) {
//        /* One byte is used for detecting the full condition. */
//        rb->size = capacity + 1;
//        rb->buf = malloc(sizeof(rbufval_t) * rb->size);
//        if (rb->buf) {
//        	ringbuf_reset(rb);
//		}
//        else {
//            free(rb);
//            return 0;
//        }
//    }
//    return rb;
//}
//
//size_t ringbuf_buffer_size(const struct ringbuf_t *rb)
//{
//    return rb->size;
//}
//
//void ringbuf_reset(ringbuf_t rb)
//{
//    rb->head = rb->tail = rb->buf;
//}
//
//void ringbuf_free(ringbuf_t *rb)
//{
//    assert(rb && *rb);
//    free((*rb)->buf);
//    free(*rb);
//    *rb = 0;
//}
//
//size_t ringbuf_capacity(const struct ringbuf_t *rb)
//{
//    return ringbuf_buffer_size(rb) - 1;
//}
//
///*
// * Return a pointer to one-past-the-end of the ring buffer's
// * contiguous buffer. You shouldn't normally need to use this function
// * unless you're writing a new ringbuf_* function.
// */
//static const uint8_t * ringbuf_end(const struct ringbuf_t *rb)
//{
//    return rb->buf + ringbuf_buffer_size(rb);
//}
//
//size_t ringbuf_bytes_free(const struct ringbuf_t *rb)
//{
//    if (rb->head >= rb->tail)
//        return ringbuf_capacity(rb) - (rb->head - rb->tail);
//    else
//        return rb->tail - rb->head - 1;
//}
//
//size_t ringbuf_bytes_used(const struct ringbuf_t *rb)
//{
//    return ringbuf_capacity(rb) - ringbuf_bytes_free(rb);
//}
//
//int ringbuf_is_full(const struct ringbuf_t *rb)
//{
//    return ringbuf_bytes_free(rb) == 0;
//}
//
//int
//ringbuf_is_empty(const struct ringbuf_t *rb)
//{
//    return ringbuf_bytes_free(rb) == ringbuf_capacity(rb);
//}
//
//const void *
//ringbuf_tail(const struct ringbuf_t *rb)
//{
//    return rb->tail;
//}
//
//const void *
//ringbuf_head(const struct ringbuf_t *rb)
//{
//    return rb->head;
//}
//
///*
// * Given a ring buffer rb and a pointer to a location within its
// * contiguous buffer, return the a pointer to the next logical
// * location in the ring buffer.
// */
//static rbufval_t *
//ringbuf_nextp(ringbuf_t rb, const uint8_t *p)
//{
//    /*
//     * The assert guarantees the expression (++p - rb->buf) is
//     * non-negative; therefore, the modulus operation is safe and
//     * portable.
//     */
//    assert((p >= rb->buf) && (p < ringbuf_end(rb)));
//    return rb->buf + ((++p - rb->buf) % ringbuf_buffer_size(rb));
//}
//
//ssize_t ringbuf_read(int fd, ringbuf_t rb, size_t count)
//{
//    const uint8_t *bufend = ringbuf_end(rb);
//    size_t nfree = ringbuf_bytes_free(rb);
//
//    /* don't write beyond the end of the buffer */
//    assert(bufend > rb->head);
//    count = MIN(bufend - rb->head, count);
//    ssize_t n = read(fd, rb->head, count);
//    if (n > 0) {
//        assert(rb->head + n <= bufend);
//        rb->head += n;
//
//        /* wrap? */
//        if (rb->head == bufend)
//            rb->head = rb->buf;
//
//        /* fix up the tail pointer if an overflow occurred */
//        if (n > nfree) {
//            rb->tail = ringbuf_nextp(rb, rb->head);
//            assert(ringbuf_is_full(rb));
//        }
//    }
//
//    return n;
//}
//
//ssize_t ringbuf_write(int fd, ringbuf_t rb, size_t count)
//{
//    size_t bytes_used = ringbuf_bytes_used(rb);
//    if (count > bytes_used)
//        return 0;
//
//    const uint8_t *bufend = ringbuf_end(rb);
//    assert(bufend > rb->head);
//    count = MIN(bufend - rb->tail, count);
//    ssize_t n = write(fd, rb->tail, count);
//    if (n > 0) {
//        assert(rb->tail + n <= bufend);
//        rb->tail += n;
//
//        /* wrap? */
//        if (rb->tail == bufend)
//            rb->tail = rb->buf;
//
//        assert(n + ringbuf_bytes_used(rb) == bytes_used);
//    }
//
//    return n;
//}
