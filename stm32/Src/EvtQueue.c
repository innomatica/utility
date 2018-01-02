/**
 * \file
 * \author	<a href="http://www.innomatic.ca">innomatic</a>
 * \copyright <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.
 */

#include "EvtQueue.h"

#define ADVANCE_QPTR(x)     ((x+1) % EVT_QDEPTH)

static struct
{
	uint8_t buff[EVT_QDEPTH][EVT_QWIDTH];
	uint8_t head;
	uint8_t tail;
} evt_queue;


/**
 * Append a new event at the end of the queue. If the queue is full, then
 * the event is ignored and it returns with false.
 *
 * \param  event data in an array of uint8_t
 * \return false if the queue is full
 */
bool Evt_EnQueue(uint8_t *event)
{
	unsigned i;
	uint8_t next = ADVANCE_QPTR(evt_queue.head);

	// queue is full
	if(next == evt_queue.tail)
	{
		// event will be lost
		return false;
	}

	// copy event bytes into the buffer
	for(i = 0; i < EVT_QWIDTH; i++)
	{
		evt_queue.buff[evt_queue.head][i] = event[i];
	}
	// move to the next positition
	evt_queue.head = next;

	return true;
}

/**
 * Retrieve the oldest event from the queue. If the return value is false
 * the retrieved event data should be ignored.
 *
 * \param  event data in an array of uint8_t
 * \return false if the queue is empty
 */
bool Evt_DeQueue(uint8_t *event)
{
	uint8_t i;

	// queue is empty
	if(evt_queue.tail == evt_queue.head)
	{
		return false;
	}

	// copy event bytes into the buffer
	for(i = 0; i < EVT_QWIDTH; i++)
	{
		event[i] = evt_queue.buff[evt_queue.tail][i];
	}
	// move to the next position
	evt_queue.tail = ADVANCE_QPTR(evt_queue.tail);

	return true;
}

/**
 * The tail and the head pointers are set to zero. This will invalidate all
 * the data in the queue.
 */
void Evt_InitQueue(void)
{
	// clear queue by resetting the pointers
	evt_queue.head = evt_queue.tail = 0;
}