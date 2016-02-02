#include "reader.h"
#include "reader_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



reader_t* reader_initialize(char name[],int delay, int maxsize){
		reader_t* new_reader=(reader_t*)malloc(sizeof(reader_t));
		new_reader->name=name;
		new_reader->processing_time=delay;
		new_reader->buffer=reader_buffer_initialize(maxsize);
		return new_reader;
}

void reader_destroy(reader_t* reader){
	if(reader_list_removeReader((void*)reader)){
		free(reader);
	}
}

void *reader_consume(void* reader){
	while(1){
		msg_t* message= reader_buffer_consume(((reader_t*)reader)->buffer);
		if(message!=NULL && message!=&POISON_PILL_MSG && message->content!=NULL){
			printf("%s ha letto %d\n", ((reader_t*)reader)->name, (int)message->content);
			sleep(((reader_t*)reader)->processing_time);
		}
		else{
			break;
		}
	}
	reader_destroy((reader_t*)reader);
	pthread_exit(NULL);
}

int reader_insert(reader_t* reader, msg_t* message){
	int result=reader_buffer_insert(reader->buffer, message);
	return result;
}
