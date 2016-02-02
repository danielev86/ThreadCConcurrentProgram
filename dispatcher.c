#include "dispatcher.h"

void *dispatch_message(int max){

	
	while(!reader_list_isEmpty()){
		
		msg_t* message=provider_buffer_read();
		reader_list_delete_reader(max);
		if (message==&POISON_PILL_MSG){
			reader_list_kill();
			break;
		}
		else if (message!=NULL){
			reader_list_broadcast(message);
			sleep(2);
		}else break;

	}
	pthread_exit(NULL);
}


