#include "CUnit/CUnit.h"
#include <stdlib.h>



#include "../reader.h"
#include "../reader_buffer.h"
#include "../reader_list.h"
#include "testReader.h"
#include "bufferDomainAssertion.h"
#include <pthread.h>

reader_t* reader_ritardo5_bufferUnitario;
reader_t* reader_ritardoUnitario_bufferGenerico;
msg_t* messages[8];
pthread_t reader;

void reader_fixture(reader_t* reader, msg_t* messages[]){
	int i;
	for(i=0;i<8;i++){
		reader_insert(reader_ritardoUnitario_bufferGenerico,messages[i]);
	}
}

int reader_before1(){
	reader_list_initialize();
	reader_ritardo5_bufferUnitario=reader_initialize("Reader 1",5,1);
	reader_ritardoUnitario_bufferGenerico=reader_initialize("Reader 2",1,8);
	int i;
	for (i=0;i<8;i++){
		messages[i]=msg_init((void*)(i*10));
	}
	reader_list_addReader((void*)reader_ritardo5_bufferUnitario);
	reader_list_addReader((void*)reader_ritardoUnitario_bufferGenerico);
	return 0;
}
int reader_after1(){
	reader_destroy(reader_ritardo5_bufferUnitario);
	reader_destroy(reader_ritardoUnitario_bufferGenerico);
	reader_list_destroy();
	return 0;
}

int reader_before2(){
	reader_list_initialize();
	reader_ritardoUnitario_bufferGenerico=reader_initialize("Reader 2",1,8);
	int i;
	for (i=0;i<8;i++){
		messages[i]=msg_init((void*)(i*30+1));
	}
	reader_list_addReader((void*)reader_ritardoUnitario_bufferGenerico);
	return 0;
}
int reader_after2(){
	reader_destroy(reader_ritardoUnitario_bufferGenerico);
	reader_list_destroy();
	return 0;
}

void test_reader_insert_oneMessage(){
	reader_insert(reader_ritardo5_bufferUnitario,messages[0]);
	CU_ASSERT_PTR_NULL(reader_ritardo5_bufferUnitario->buffer->read_buffer->T);
	CU_ASSERT_PTR_EQUAL(reader_ritardo5_bufferUnitario->buffer->read_buffer->queue[reader_ritardo5_bufferUnitario->buffer->read_buffer->T].content,messages[0]->content);
}

void test_reader_insert_eightMessage(){
	int i;
	for(i=0;i<8;i++){
		reader_insert(reader_ritardoUnitario_bufferGenerico,messages[i]);
	}
	ASSERT_BUFFER_EQUALS(reader_ritardoUnitario_bufferGenerico->buffer->read_buffer,messages);
}

void test_reader_consume(){
	reader_fixture(reader_ritardoUnitario_bufferGenerico,messages);
	ASSERT_BUFFER_EQUALS(reader_ritardoUnitario_bufferGenerico->buffer->read_buffer,messages);
	pthread_create(&reader,NULL,reader_consume,(void*)reader_ritardoUnitario_bufferGenerico);
	sleep(10);
	ASSERT_BUFFER_EMPTY(reader_ritardoUnitario_bufferGenerico->buffer->read_buffer);
	pthread_join(reader,NULL);
}
