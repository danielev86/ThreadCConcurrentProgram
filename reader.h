#ifndef READER_H_
#define READER_H_

#include "reader_buffer.h"

typedef struct reader{
	char* name;
	int processing_time;
	reader_buffer_t* buffer;
} reader_t;

reader_t* reader_initialize(char name[], int delay, int maxsize);
void reader_destroy(reader_t* reader);
void *reader_consume(void* reader);
int reader_insert(reader_t* reader, msg_t* message);
#endif
