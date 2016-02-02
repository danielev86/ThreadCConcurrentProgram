#ifndef PROVIDER_BUFFER_H_
#define PROVIDER_BUFFER_H_

#include "buffer.h"
#include "poison_pill.h"



buffer_t* provider_buffer;

/*Inizializza il buffer*/
void provider_buffer_initialize(int n);
void provider_buffer_destroy();
void provider_buffer_insert(msg_t* message);
msg_t* provider_buffer_read();

#endif
