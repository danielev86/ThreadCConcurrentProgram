#include "accepter.h"
#include "reader_list.h"
#include <stdlib.h>
#include <math.h>




void submitRequest(buffer_t* requests, char name[]){
	msg_t* request= accepter_buffer_delete(requests);
	if(request!=&POISON_PILL_MSG){
		int delay=rand()%20;
		reader_t* reader=reader_initialize(name, delay, 10);
		reader_list_addReader((void*)reader);
	}
	free(request);
}
