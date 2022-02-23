#ifndef __POPPER_H__
#define __POPPER_H__

#include "blocking_queue.h"

// Starts popper thread.
void start_popper(blocking_queue_t* queue, const bool join);

#endif