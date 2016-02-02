#include "CUnit/CUnit.h"
#include <stdlib.h>

#include "../provider.h"
#include "../provider_buffer.h"
#include "testProvider.h"
#include "bufferDomainAssertion.h"
#include <pthread.h>



struct messaggi content;
//int messages_content[7];
msg_t* testing[8];
pthread_t provider;

int provider_buffer_before(){
	int i;
	content.size=7;
	for (i=0;i<7;i++){
		content.messages[i]=msg_init((void*)((i+1)*20));
		testing[i]=msg_init((void*)((i+1)*20));
	}
	testing[7]=(msg_t*)&POISON_PILL_MSG;
	return 0;
}

int provider_buffer_after(){
	return 0;
}


void test_provider_buffer_insert_oneMessage(){
	provider_buffer_initialize(8);
	provider_buffer_insert(testing[0]);
	CU_ASSERT_PTR_NULL(provider_buffer->T);
	CU_ASSERT_PTR_EQUAL(provider_buffer->queue[provider_buffer->T].content,testing[0]->content);
	provider_buffer_destroy();
}

void test_provider_buffer_insert_eightMessage(){
	provider_buffer_initialize(8);
	int i;
	for(i=0;i<8;i++){
		provider_buffer_insert(testing[i]);
	}
	ASSERT_BUFFER_EQUALS(provider_buffer,testing);
	provider_buffer_destroy();
}

void test_createMessageSequence(){
	int dim=8;
	provider_buffer_initialize(dim);
	pthread_create(&provider,NULL,create_message_sequence,(void*)&content);
	sleep(10);
	ASSERT_BUFFER_EQUALS(provider_buffer,testing);
	provider_buffer_destroy();
	pthread_join(provider,NULL);
}

void test_provider_buffer_insertAndRead(){
	provider_buffer_initialize(8);
	provider_buffer_insert(testing[0]);
	msg_t* retrived=provider_buffer_read();
	CU_ASSERT_PTR_NOT_NULL(provider_buffer->T);
	CU_ASSERT_PTR_EQUAL(retrived->content,testing[0]->content);
	provider_buffer_destroy();
}
