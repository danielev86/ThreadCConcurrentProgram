#ifndef ACCEPTER_H_
#define ACCEPTER_H_



#include "accepter_buffer.h"
#include <pthread.h>

void submitRequest(buffer_t* requests, char name[]);

#endif
