#include "provider_buffer.h"
#include <stdlib.h>



void provider_buffer_initialize(int n){
	provider_buffer=buffer_init(n);
}

void provider_buffer_destroy(){
	buffer_destroy(provider_buffer);
}

void provider_buffer_insert(msg_t* message){
	msg_t* msg= put_bloccante(provider_buffer,message);
	free(msg);
}

msg_t* provider_buffer_read(){
	msg_t* message=get_non_bloccante(provider_buffer);
	return message;
}
