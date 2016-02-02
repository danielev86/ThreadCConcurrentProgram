#include "testDispatcher.h"
#include "bufferDomainAssertion.h"
#include "../dispatcher.h"
#include "../provider.h"
#include "CUnit/CUnit.h"


reader_t* readers[5];
pthread_t dispatcher;
pthread_t provider;
pthread_t readers_thread[5];
int dim=10;
struct messaggi sequence;

void clean_buffer_readers(){
	int i;
	for(i=0;i<5;i++){
		pthread_create(&readers_thread[i],NULL,reader_consume,(void*)readers[i]);
		sleep(1);
	}
	for(i=0;i<5;i++){
		pthread_join(readers_thread[i],NULL);
	}
}

void create_readers(){
	readers[0]=reader_initialize("Reader 1",1,dim);
	readers[1]=reader_initialize("Reader 2",2,dim);
	readers[2]=reader_initialize("Reader 3",4,dim);
	readers[3]=reader_initialize("Reader 4",3,dim);
	readers[4]=reader_initialize("Reader 5",5,dim);


}

void create_readers_slow(){
	readers[0]=reader_initialize("Reader 1",4,dim);
	readers[1]=reader_initialize("Reader 2",5,dim);
	readers[2]=reader_initialize("Reader 3",7,dim);
	readers[3]=reader_initialize("Reader 4",10,dim);
	readers[4]=reader_initialize("Reader 5",1,dim);


}

void add_readers(){
	int i;
	for(i=0;i<5;i++){
		reader_list_addReader((void*)readers[i]);
	}
}



int dispatcher_before(){
	reader_list_initialize();
	provider_buffer_initialize(5);
	sequence.size=4;
	sequence.messages[0]=msg_init((void*)(1));
	sequence.messages[1]=msg_init((void*)(2));
	sequence.messages[2]=msg_init((void*)(3));
	sequence.messages[3]=msg_init((void*)(4));

	return 0;

}

int dispatcher_after(){
	provider_buffer_destroy();
	reader_list_destroy();
	return 0;
}

void test_dispatch_message(){
	create_readers();
	add_readers();
	pthread_create(&provider,NULL,create_message_sequence,(void*)&sequence);
	sleep(7);
	pthread_create(&dispatcher,NULL,dispatch_message,10);

	clean_buffer_readers();
	pthread_join(dispatcher,NULL);
	ASSERT_BUFFER_EMPTY(readers[2]->buffer->read_buffer);
	sleep(1);
	pthread_join(provider,NULL);

}

void test_dispatchAndDelete_slow_reader(){
	create_readers_slow();
	add_readers();
	pthread_create(&provider,NULL,create_message_sequence,(void*)&sequence);
	sleep(7);
	pthread_create(&dispatcher,NULL,dispatch_message,6);
	//pthread_create(&dispatcher[1],NULL,delete_slow_readers,3);
	//pthread_join(dispatcher[1],NULL);

	clean_buffer_readers();
	pthread_join(dispatcher,NULL);
	ASSERT_BUFFER_EMPTY(readers[2]->buffer->read_buffer);
	sleep(1);
	pthread_join(provider,NULL);
}


