#include "accepter_buffer.h"
#include <stdio.h>
#include <stdlib.h>



void accepter_buffer_initialize(int size){
	accepter_buffer=buffer_init(size);
}

void accepter_buffer_destroy(){
	buffer_destroy(accepter_buffer);
}

void *accepter_buffer_insert(void* messaggio){
	msg_t* msg=(msg_t*)&messaggio;
	msg_t* inserted_msg=put_bloccante(accepter_buffer,msg);
	free(inserted_msg);
	pthread_exit(NULL);
}

msg_t* accepter_buffer_delete(){
	return get_bloccante(accepter_buffer);
}

void accepter_buffer_deleteAll(){
	int i;
	int size=accepter_buffer->k;
	for(i=0;i<size;i++){
		msg_t* message=accepter_buffer_delete();
		free(message);
	}
}
