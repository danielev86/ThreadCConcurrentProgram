#include "provider.h"
#include <stdlib.h>
#include <pthread.h>



void *create_message_sequence(void* content){
	struct messaggi* msg=(struct messaggi*)content;
	int n=msg->size;
	int i;
	for(i=0;i<n;i++){
		//msg_t* temp = msg_init(content);
		provider_buffer_insert(msg->messages[i]);
		sleep(1);
	}
	provider_buffer_insert((msg_t*)&POISON_PILL_MSG);
	pthread_exit(NULL);
}
