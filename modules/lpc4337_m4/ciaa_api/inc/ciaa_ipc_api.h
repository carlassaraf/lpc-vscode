/*
 * ciaa_ipc_api.h
 *
 *  Created on: Aug 27, 2022
 *      Author: carlassaraf
 */

#ifndef CIAA_IPC_API_H_
#define CIAA_IPC_API_H_

#include <string.h>
#include "ciaa_multicore_api.h"

/*
 * 	@brief IPC Queue Structure used for sync between M0 and M4.
 *
 * 	This structure provides head and tail indexes
 * 	to the uint32_t array of messages.
 */
typedef struct {
	int32_t size;				/* Size of a single item in queue */
	int32_t count;				/* Total number of elements that can be stored in the queue */
	volatile uint32_t head;		/* Head index of the queue */
	volatile uint32_t tail;		/* Tail index of the queue */
	uint8_t *data;				/* Pointer to the data */
	uint32_t valid;				/* Queue is valid only if this is #QUEUE_MAGIC_VALID */
	uint32_t reserved[2];		/* Reserved entry to keep the structure aligned */
} ipc_queue_t;

/*
 *	@brief IPC status IDs
 */
typedef enum {
	queue_timeout = -4,			/* Push/pop action timeout */
	queue_error = -3,			/* Queue was not initialized */
	queue_empty = -2,			/* Queue is empty, pop failed */
	queue_full = -1,			/* Queue is full, insert failed */
	queue_insert = 0,			/* Item successfully inserted */
	queue_valid = 1				/* Item successfully fetched */
} ipc_status_t;

/* Macro used to identify if a queue is valid */
#define QUEUE_MAGIC_VALID  0xCAB51053

/* Function prototypes */

/* Works on both cores */
void ipc_queue_init(void *data, int size, int count);

/* Next prototypes and functions compile only in M4. Implement them on your main for the M0 */
#if !defined(CORE_M0) && defined(CORE_M4)
/* This next to function need to be implemented on the M0 core */
ipc_status_t ipc_push_tout(void *data, int tout);
ipc_status_t ipc_pop_tout(void *data, int tout);

/*
 * 	@brief	Function to push the message into queue with no timeout
 *
 * 	@param	data: pointer to data to be pushed
 *
 * 	@return	queue_insert on success, queue_full or queue_error on failure
 */
static inline ipc_status_t ipc_try_push(void *data) { return ipc_push_tout(data, 0); }

/*
 * 	@brief	Function to pop the message from queue with no timeout
 *
 * 	@param	data: pointer to store popped data
 *
 * 	@return	queue_valid on success, queue_error or queue_empty on failure
 */
static inline ipc_status_t ipc_try_pop(void *data) { return ipc_pop_tout(data, 0); }

/*
 * 	@brief	Function to push the message into queue (blocking)
 *
 * 	@param	data: pointer to data to be pushed
 *
 * 	@return	queue_insert on success, queue_error on failure
 */
static inline ipc_status_t ipc_push(void *data) { return ipc_push_tout(data, -1); }

/*
 * 	@brief	Function to pop the message from queue (blocking)
 *
 * 	@param	data: pointer to store popped data
 *
 * 	@return	queue_valid on success, queue_error or queue_empty on failure
 */
static inline ipc_status_t ipc_pop(void *data) { return ipc_pop_tout(data, -1); }

#endif	/* CORE_M4 && !CORE_M0 */

/* Simple event handler for non OS implementations */
static inline int ipc_event_handler(void) {
	__WFI();
	return 0;
}

/*
 * 	@brief	Gets the number of items available
 *
 * 	@param	q: pointer to the queue to check
 *
 * 	@return	number of items available to read
 */
static inline uint32_t ipc_queue_data_count(ipc_queue_t *q) { return ((uint32_t) ((q)->head - (q)->tail)); }

/*
 * 	@brief	Gets whether the queue is full or not
 *
 * 	@param	q: pointer to the queue to check
 *
 * 	@return	true if full, false if not
 */
static inline bool ipc_queue_is_full(ipc_queue_t *q) { return (ipc_queue_data_count(q) >= (q)->count); }

/*
 *	@brief	Gets whether the queue is empty or not
 *
 *	@param	q: pointer to the queue to check
 *
 *	@return	true if empty, false if not
 */
static inline bool ipc_queue_is_empty(ipc_queue_t *q) { return ((q)->head == (q)->tail); }

/*
 *	@brief	Gets whether the queue is valid
 *
 *	@param	q: pointer to the queue to check
 *
 *	@return	true if valid, false if not
 */
static inline bool ipc_queue_is_valid(ipc_queue_t *q) { return ((q)->valid == QUEUE_MAGIC_VALID); }

/*
 *	@brief	Pushes data to the pointed queue
 *
 *	@param	q: pointer to the queue
 *	@param	data: pointer data to push
 *
 *	@return	queue_insert on success
 */
static inline ipc_status_t ipc_queue_push(ipc_queue_t *q, void *data) {
	q->data = data;
	memcpy(q->data + ((q->head & (q->count - 1)) * q->size), data, q->size);
	q->head++;
	return queue_insert;
}

/*
 *	@brief	Pops data from the pointed queue
 *
 *	@param	q: pointer to the queue
 *	@param	data: pointer to store the popped data
 *
 *	@return	queue_valid on success
 */
static inline ipc_status_t ipc_queue_pop(ipc_queue_t *q, void *data) {
	memcpy(data, q->data + ((q->tail & (q->count - 1)) * q->size), q->size);
	q->tail++;
	data = q->data;
	return queue_valid;
}

#endif /* CIAA_IPC_API_H_ */
