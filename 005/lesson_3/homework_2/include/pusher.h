#ifndef __PUSHER_H__
#define __PUSHER_H__

#include "blocking_queue.h"

// Starts pusher thread.
void start_pusher(blocking_queue_t* queue, const bool join);

#endif