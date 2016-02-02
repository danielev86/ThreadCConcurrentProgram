#ifndef PROVIDER_H_
#define PROVIDER_H_

#include <pthread.h>
#include "poison_pill.h"
#include "message.h"
#include "buffer.h"
#include "provider_buffer.h"



struct messaggi{
	int size;
	msg_t* messages[];
};

/*Crea una sequenza di n messaggi, terminata da una poison pill*/
void *create_message_sequence(void* content);

#endif
