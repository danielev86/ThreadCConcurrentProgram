/** Implementazione del tipo messaggio **/

#include <stdlib.h>
#include <stdio.h>
#include "message.h"



/** Costruttore del messaggio **/

msg_t* msg_init(void* content) {
	msg_t* new_msg = (msg_t*)malloc(sizeof(msg_t));
	new_msg->content = content;
	new_msg->msg_init = msg_init;
	new_msg->msg_destroy = msg_destroy;
	new_msg->msg_copy = msg_copy;

	return new_msg;
}

/** Distruttore del messaggio **/

void msg_destroy(msg_t* msg){
	free(msg->content);
	free(msg);
}

/** Costruttore di copia del messaggio **/

msg_t* msg_copy(msg_t* msg){
	return msg_init(msg->content);
}
