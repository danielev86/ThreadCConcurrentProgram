#include "CUnit/CUnit.h"
#include <stdlib.h>

#include "../accepter.h"
#include "../accepter_buffer.h"
#include "../reader_list.h"
#include "testAccepter.h"
#include "bufferDomainAssertion.h"
#include "reader_listDomainAssertion.h"
#include <pthread.h>



msg_t* messages[8];
reader_t* reader1;
pthread_t requester[8];

void accepter_buffer_fixture_1(msg_t* messages[]){
	int i;
	for(i=0;i<8;i++){
		pthread_create(&requester[i],NULL,accepter_buffer_insert,(void*)&messages[i]);
	}
		for(i=0;i<8;i++){
		pthread_join(requester[i],NULL);
	}
}

void accepter_buffer_fixture_2(msg_t* poison_pill){
	pthread_create(&requester[0],NULL,accepter_buffer_insert,(void*)&poison_pill);
	pthread_join(requester[0],NULL);

}

void reader_list_fixture(reader_t* reader){
	reader1= reader_initialize("Reader 1",1,10);
	reader_list_addReader((void*)reader1);
}

int accepter_buffer_before(){
	int i;
	for (i=0;i<8;i++){
		messages[i]=msg_init((void*)(i*10+1));
	}
	return 0;
}

int accepter_buffer_after(){

	return 0;
}

void test_accepter_buffer_insert_oneMessage(){
	accepter_buffer_initialize(8);
	pthread_create(&requester[0],NULL,accepter_buffer_insert,(void*)&messages[0]);
	pthread_join(requester[0],NULL);
	CU_ASSERT_PTR_NULL(accepter_buffer->T);
	CU_ASSERT_PTR_EQUAL(accepter_buffer->k,1);
	accepter_buffer_destroy();
}

void test_accepter_buffer_insert_eightMessage(){
	accepter_buffer_initialize(8);
	int i;
	for(i=0;i<8;i++){
		pthread_create(&requester[i],NULL,accepter_buffer_insert,(void*)&messages[i]);
	}
	for(i=0;i<8;i++){
		pthread_join(requester[i],NULL);
	}
	CU_ASSERT_EQUAL(accepter_buffer->k,8);	
	accepter_buffer_destroy();
}

void test_accepter_buffer_delete(){
	accepter_buffer_initialize(8);
	accepter_buffer_fixture_1(messages);
	int i,j;
	msg_t* testing[7];
	for(i=0, j=1;i<7,j<8;i++,j++){
		testing[i]=messages[j];
	}
	msg_t* retrieve= accepter_buffer_delete();
	CU_ASSERT_PTR_NOT_NULL(retrieve->content);
	CU_ASSERT_EQUAL(accepter_buffer->k,7);
	accepter_buffer_destroy();
}

void test_accepter_buffer_deleteAll(){
	accepter_buffer_initialize(8);
	accepter_buffer_fixture_1(messages);
	CU_ASSERT_EQUAL(accepter_buffer->k,8);
	accepter_buffer_deleteAll();
	CU_ASSERT_EQUAL(accepter_buffer->k, 0);
	ASSERT_BUFFER_NOTEQUALS(accepter_buffer,messages);
	accepter_buffer_destroy();
}

void test_accepter_submitRequestNormal(){
	accepter_buffer_initialize(8);
	accepter_buffer_fixture_1(messages);
	reader_list_initialize();
	submitRequest(accepter_buffer,"Prova");
	ASSERT_READER_INSERTED(reader_list);
	reader_list_destroy();
	accepter_buffer_destroy();
}

void test_accepter_submitRequestPoisonPill(){
	accepter_buffer_initialize(8);
	accepter_buffer_fixture_2((msg_t*)&POISON_PILL_MSG);
	reader_list_initialize();
	reader_list_fixture(reader1);
	submitRequest(accepter_buffer,"Kill");
	ASSERT_READER_NOT_INSERTED(reader_list);
	ASSERT_BUFFER_CONTAINS_ONLY(reader1->buffer->read_buffer, (msg_t*)&POISON_PILL_MSG);
	reader_list_destroy();
	accepter_buffer_destroy();
}
