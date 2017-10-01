#ifndef INCLUDED_RINGBUF_H
#define INCLUDED_RINGBUF_H

/*
 * ringbuf.h - C ring buffer (FIFO) interface.
 *
 * Written in 2011 by Drew Hess <dhess-src@bothan.net>.
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to
 * the public domain worldwide. This software is distributed without
 * any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

/*
 * A byte-addressable ring buffer FIFO implementation.
 *
 * The ring buffer's head pointer points to the starting location
 * where data should be written when copying data *into* the buffer
 * (e.g., with ringbuf_read). The ring buffer's tail pointer points to
 * the starting location where data should be read when copying data
 * *from* the buffer (e.g., with ringbuf_write).
 */

#include <stddef.h>
#include <sys/types.h>



typedef struct ringbuf_t {
	float *buffer;
	size_t head_index;
    size_t capacity;
} ringbuf_t;

///*
// * Create a new ring buffer with the given capacity (usable
// * bytes). Note that the actual internal buffer size may be one or
// * more bytes larger than the usable capacity, for bookkeeping.
// *
// * Returns the new ring buffer object, or 0 if there's not enough
// * memory to fulfill the request for the given capacity.
// */
//ringbuf_t ringbuf_new(size_t capacity);
//
///*
// * The size of the internal buffer, in bytes. One or more bytes may be
// * unusable in order to distinguish the "buffer full" state from the
// * "buffer empty" state.
// *
// * For the usable capacity of the ring buffer, use the
// * ringbuf_capacity function.
// */
//size_t ringbuf_buffer_size(const struct ringbuf_t *rb);
//
///*
// * Deallocate a ring buffer, and, as a side effect, set the pointer to
// * 0.
// */
//void ringbuf_free(ringbuf_t *rb);
//
///*
// * Reset a ring buffer to its initial state (empty).
// */
//void ringbuf_reset(ringbuf_t rb);
//
///*
// * The usable capacity of the ring buffer, in bytes. Note that this
// * value may be less than the ring buffer's internal buffer size, as
// * returned by ringbuf_buffer_size.
// */
//size_t ringbuf_capacity(const struct ringbuf_t *rb);
//
//int ringbuf_is_full(const struct ringbuf_t *rb);
//
//int ringbuf_is_empty(const struct ringbuf_t *rb);
//
///*
// * Const access to the head and tail pointers of the ring buffer.
// */
//const void *ringbuf_tail(const struct ringbuf_t *rb);
//
//const void *ringbuf_head(const struct ringbuf_t *rb);
//
//
#endif /* INCLUDED_RINGBUF_H */
