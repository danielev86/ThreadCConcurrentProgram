#include "../buffer.h"
#include "testBuffer.h"
#include "CUnit/CUnit.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>



/** Dichiarazione delle variabili usate nei test **/

buffer_t* buffer_unitario_vuoto;
buffer_t* buffer_unitario_pieno;
buffer_t* buffer_generico_vuoto;
buffer_t* buffer_generico_pieno;
char* stringa_test;
char* stringa_errore;
char* stringhe_test[10];
pthread_t thread_prods[10];
pthread_t thread_conss[10];


int buffer_unitario_vuoto_before(){
	stringa_test="Test";
	stringa_errore="Errore";
	return 0;
}

int buffer_unitario_vuoto_after(){
	stringa_test=stringa_errore="";
	return 0;
}

int buffer_unitario_pieno_before(){
	stringa_test="Test";
	stringa_errore="Errore";
	return 0;
}

int buffer_unitario_pieno_after(){
	stringa_test=stringa_errore="";
	return 0;
}

int buffer_generico_vuoto_before(){
	stringhe_test[0]="Test0";
	stringhe_test[1]="Test1";
	stringhe_test[2]="Test2";
	stringhe_test[3]="Test3";
	stringhe_test[4]="Test4";
	stringhe_test[5]="Test5";
	stringhe_test[6]="Test6";
	stringhe_test[7]="Test7";
	stringhe_test[8]="Test8";
	stringhe_test[9]="Test9";
	return 0;
}

int buffer_generico_vuoto_after(){
	stringhe_test[0]="";
	stringhe_test[1]="";
	stringhe_test[2]="";
	stringhe_test[3]="";
	stringhe_test[4]="";
	stringhe_test[5]="";
	stringhe_test[6]="";
	stringhe_test[7]="";
	stringhe_test[8]="";
	stringhe_test[9]="";
	return 0;
}

int buffer_generico_pieno_before(){
	stringhe_test[0]="Test0";
	stringhe_test[1]="Test1";
	stringhe_test[2]="Test2";
	stringhe_test[3]="Test3";
	stringhe_test[4]="Test4";
	stringhe_test[5]="Test5";
	stringhe_test[6]="Test6";
	stringhe_test[7]="Test7";
	stringhe_test[8]="Test8";
	stringhe_test[9]="Test9";
	return 0;
}

int buffer_generico_pieno_after(){
	stringhe_test[0]="";
	stringhe_test[1]="";
	stringhe_test[2]="";
	stringhe_test[3]="";
	stringhe_test[4]="";
	stringhe_test[5]="";
	stringhe_test[6]="";
	stringhe_test[7]="";
	stringhe_test[8]="";
	stringhe_test[9]="";
	return 0;
}

void buffer_unitario_pieno_fixture(){
	msg_t* temp=msg_init(stringa_test);
	pthread_mutex_lock(&(buffer_unitario_pieno->uso_d));
	buffer_unitario_pieno->queue[buffer_unitario_pieno->D]=*temp;
	buffer_unitario_pieno->k=1;
	pthread_mutex_unlock(&(buffer_unitario_pieno->uso_d));
}

void buffer_generico_pieno_fixture(){
	int i;
	for(i=0;i<10;i++){
		msg_t* temp=msg_init(stringhe_test[i]);
		pthread_mutex_lock(&(buffer_generico_pieno->uso_d));
		buffer_generico_pieno->queue[buffer_generico_pieno->D]=*temp;
		buffer_generico_pieno->k++;
		buffer_generico_pieno->D=(buffer_generico_pieno->D+1)%(buffer_generico_pieno->size);
		pthread_mutex_unlock(&(buffer_generico_pieno->uso_d));
		free(temp);
	}
}

void* put_bloccante_buffer_unitario_vuoto_messaggio_singolo (){
	msg_t* expected_msg = msg_init((void*)stringa_test);
	msg_t* inserted_msg= put_bloccante(buffer_unitario_vuoto,expected_msg);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg,expected_msg);
	CU_ASSERT_PTR_EQUAL(inserted_msg->content,expected_msg->content)
	sleep(1);
	pthread_exit(NULL);
}

void* put_non_bloccante_buffer_unitario_vuoto_messaggio_singolo (){
	msg_t* expected_msg = msg_init((void*)stringa_test);
	msg_t* inserted_msg= put_non_bloccante(buffer_unitario_vuoto,expected_msg);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg,expected_msg);
	if(inserted_msg!=NULL){
		CU_ASSERT_PTR_EQUAL(inserted_msg->content,expected_msg->content);
	}
	else CU_ASSERT_PTR_NULL(inserted_msg); 
	sleep(1);
	pthread_exit(NULL);
}

void* get_bloccante_buffer_unitario_vuoto_messaggio_singolo(){
	msg_t* retrived_msg = get_bloccante(buffer_unitario_vuoto);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringa_test);
	sleep(1);
	pthread_exit(NULL);
}

void* get_non_bloccante_buffer_unitario_vuoto_messaggio_singolo(){
	msg_t* retrived_msg = get_non_bloccante(buffer_unitario_vuoto);
	if (retrived_msg!=NULL){
		CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringa_test);
	}
	else CU_ASSERT_PTR_NULL(retrived_msg);
	sleep(1);
	pthread_exit(NULL);
}

void* put_bloccante_buffer_unitario_vuoto_nonInserisceMessaggio (){
	msg_t* expected_msg = msg_init((void*)stringa_errore);
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,buffer_unitario_vuoto->size);
	CU_ASSERT_PTR_NOT_EQUAL(buffer_unitario_vuoto->queue[buffer_unitario_vuoto->D].content,expected_msg->content);
	sleep(1);
	pthread_exit(NULL);
}

void* put_non_bloccante_buffer_unitario_vuoto_nonInserisceMessaggio (){
	msg_t* expected_msg = msg_init((void*)stringa_errore);
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,buffer_unitario_vuoto->size);
	msg_t* inserted_msg = put_non_bloccante(buffer_unitario_vuoto, expected_msg);
	CU_ASSERT_PTR_NULL(inserted_msg);
	CU_ASSERT_PTR_NOT_EQUAL(buffer_unitario_vuoto->queue[buffer_unitario_vuoto->D].content,expected_msg->content);
	sleep(1);
	pthread_exit(NULL);
}

void* get_bloccante_buffer_unitario_pieno() {
	msg_t* retrived_msg = get_bloccante(buffer_unitario_pieno);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringa_test);
	sleep(1);
	pthread_exit(NULL);
}

void* get_bloccante_buffer_unitario_pieno_nonConsumaMessaggio(){
	CU_ASSERT_EQUAL(buffer_unitario_pieno->k,0);
	sleep(1);
	pthread_exit(NULL);
}

void* get_non_bloccante_buffer_unitario_pieno() {
	msg_t* retrived_msg = get_non_bloccante(buffer_unitario_pieno);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringa_test);
	sleep(1);
	pthread_exit(NULL);
}

void* get_non_bloccante_buffer_unitario_pieno_nonConsumaMessaggio(){
	CU_ASSERT_EQUAL(buffer_unitario_pieno->k,0);
	msg_t* retrived_msg= get_non_bloccante(buffer_unitario_pieno);
	CU_ASSERT_PTR_NULL(retrived_msg);
	sleep(1);
	pthread_exit(NULL);
}

void* put_bloccante_buffer_generico_vuoto_messaggio_singolo(void* i){
	msg_t* expected_msg = msg_init((void*)stringhe_test[(int)i]);
	msg_t* inserted_msg= put_bloccante(buffer_generico_vuoto,expected_msg);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg,expected_msg);
	CU_ASSERT_PTR_EQUAL(inserted_msg->content,expected_msg->content)
	sleep(1);
	pthread_exit(NULL);	
}

void* get_bloccante_buffer_generico_vuoto_messaggio_singolo(void* i){
	msg_t* retrived_msg = get_bloccante(buffer_generico_vuoto);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringhe_test[(int) i]);
	sleep(1);
	pthread_exit(NULL);
}

void* put_bloccante_buffer_generico_pieno_nessunMessaggio(){
	CU_ASSERT_EQUAL(buffer_generico_pieno->k,buffer_generico_pieno->size);
	sleep(1);
	pthread_exit(NULL);
}

void* put_non_bloccante_buffer_generico_pieno_nessunMessaggio(void* i){
	msg_t* expected_msg = msg_init((void*)stringhe_test[(int)i]);
	msg_t* inserted_msg= put_non_bloccante(buffer_generico_pieno,expected_msg);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg,expected_msg);
	CU_ASSERT_PTR_NULL(inserted_msg);	
	CU_ASSERT_EQUAL(buffer_generico_pieno->k,buffer_generico_pieno->size);
	sleep(1);
	pthread_exit(NULL);
}

void* get_bloccante_buffer_generico_pieno_messaggio_singolo(void* i){
	msg_t* retrived_msg = get_bloccante(buffer_generico_vuoto);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringhe_test[(int) i]);
	sleep(1);
	pthread_exit(NULL);
}

void* get_non_bloccante_buffer_generico_pieno_messaggio_singolo(void* i){
	msg_t* retrived_msg = get_non_bloccante(buffer_generico_pieno);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringhe_test[(int) i]);
	sleep(1);
	pthread_exit(NULL);
}

void* put_non_bloccante_buffer_generico_vuoto_messaggio_singolo(void* i){
	msg_t* expected_msg = msg_init((void*)stringhe_test[(int)i]);
	msg_t* inserted_msg= put_non_bloccante(buffer_generico_vuoto,expected_msg);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg,expected_msg);
	CU_ASSERT_PTR_EQUAL(inserted_msg->content,expected_msg->content)
	sleep(1);
	pthread_exit(NULL);	
}

void* get_non_bloccante_buffer_generico_vuoto_messaggio_singolo(void* i){
	msg_t* retrived_msg = get_non_bloccante(buffer_generico_vuoto);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringhe_test[(int) i]);
	sleep(1);
	pthread_exit(NULL);
}

void* put_bloccante_buffer_generico_vuoto_messaggioDoppio (void* i){
	msg_t* expected_msg_1 = msg_init((void*)stringhe_test[(int)i]);
	msg_t* inserted_msg_1= put_bloccante(buffer_generico_vuoto,expected_msg_1);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg_1,expected_msg_1);
	CU_ASSERT_PTR_EQUAL(inserted_msg_1->content,expected_msg_1->content);
	sleep(1);
	msg_t* expected_msg_2 = msg_init((void*)stringhe_test[((int)i)+5]);
	msg_t* inserted_msg_2= put_bloccante(buffer_generico_vuoto,expected_msg_2);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg_2,expected_msg_2);
	CU_ASSERT_PTR_EQUAL(inserted_msg_2->content,expected_msg_2->content);
	sleep(1);
	pthread_exit(NULL);
}

void* put_non_bloccante_buffer_generico_vuoto_messaggioDoppio (void* i){
	msg_t* expected_msg_1 = msg_init((void*)stringhe_test[(int)i]);
	msg_t* inserted_msg_1= put_non_bloccante(buffer_generico_vuoto,expected_msg_1);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg_1,expected_msg_1);
	CU_ASSERT_PTR_EQUAL(inserted_msg_1->content,expected_msg_1->content);
	sleep(1);
	msg_t* expected_msg_2 = msg_init((void*)stringhe_test[((int)i)+5]);
	msg_t* inserted_msg_2= put_non_bloccante(buffer_generico_vuoto,expected_msg_2);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg_2,expected_msg_2);
	CU_ASSERT_PTR_EQUAL(inserted_msg_2->content,expected_msg_2->content);
	sleep(1);
	pthread_exit(NULL);
}

void create_concurrent_producers_blocking_oneMessage(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_prods[i],NULL,put_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
		sleep(1);
	}
}

void create_concurrent_producers_non_blocking_oneMessage(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_prods[i],NULL,put_non_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
		sleep(1);
	}
}

void create_concurrent_consumers_blocking_oneMessage(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_conss[i],NULL,get_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
		sleep(1);
	}
}

void create_concurrent_producers_blocking_oneMessage_buffer_generico_pieno(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_prods[i],NULL,put_bloccante_buffer_generico_pieno_nessunMessaggio,NULL);
		sleep(1);
	}
}

void create_concurrent_consumers_blocking_oneMessage_buffer_generico_pieno(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_conss[i],NULL,get_bloccante_buffer_generico_pieno_messaggio_singolo,(void*)i);
		sleep(1);
	}
}

void create_concurrent_consumers_non_blocking_oneMessage(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_conss[i],NULL,get_non_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
		sleep(1);
	}
}

void create_concurrent_producers_non_blocking_oneMessage_buffer_generico_pieno(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_prods[i],NULL,put_non_bloccante_buffer_generico_pieno_nessunMessaggio,(void*)i);
		sleep(1);
	}
}

void create_concurrent_consumers_non_blocking_oneMessage_buffer_generico_pieno(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_conss[i],NULL,get_non_bloccante_buffer_generico_pieno_messaggio_singolo,(void*)i);
		sleep(1);
	}
}

void create_producerAndConsumer_blocking(){
	pthread_create(&thread_prods[0],NULL,put_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
	pthread_create(&thread_conss[0],NULL,get_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
}

void create_producerAndConsumer_non_blocking(){
	pthread_create(&thread_prods[0],NULL,put_non_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
	pthread_create(&thread_conss[0],NULL,get_non_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
}

void create_consumerAndProducer_blocking(){
	pthread_create(&thread_conss[0],NULL,get_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
	pthread_create(&thread_prods[0],NULL,put_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
}

void create_consumerAndProducer_non_blocking(){
	pthread_create(&thread_conss[0],NULL,get_non_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
	pthread_create(&thread_prods[0],NULL,put_non_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
}

void join_threads_oneMessage(){
	int i;
	for(i=0;i<10;i++){
		pthread_join(thread_prods[i],NULL);
		pthread_join(thread_conss[i],NULL);
	}
}

void join_producerAndConsumer(){
	pthread_join(thread_prods[0],NULL);
	pthread_join(thread_conss[0],NULL);
}

void create_concurrent_producers_blocking_onlyOneMessage(){
	int i;
	pthread_create(&thread_prods[0],NULL,put_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
	for(i=1;i<10;i++){
		pthread_create(&thread_prods[i],NULL,put_bloccante_buffer_unitario_vuoto_nonInserisceMessaggio,NULL);
		sleep(1);
	}
}

void create_concurrent_producers_non_blocking_onlyOneMessage(){
	int i;
	pthread_create(&thread_prods[0],NULL,put_non_bloccante_buffer_unitario_vuoto_messaggio_singolo,NULL);
	sleep(1);
	for(i=1;i<10;i++){
		pthread_create(&thread_prods[i],NULL,put_non_bloccante_buffer_unitario_vuoto_nonInserisceMessaggio,NULL);
		sleep(1);
	}
}

void create_concurrent_consumers_blocking_onlyOneMessage(){
	int i;
	pthread_create(&thread_conss[0],NULL,get_bloccante_buffer_unitario_pieno,NULL);
	sleep(1);
	for(i=1;i<10;i++){
		pthread_create(&thread_conss[i],NULL,get_bloccante_buffer_unitario_pieno_nonConsumaMessaggio,NULL);
		sleep(1);
	}
}

void create_concurrent_consumers_non_blocking_onlyOneMessage(){
	int i;
	pthread_create(&thread_conss[0],NULL,get_non_bloccante_buffer_unitario_pieno,NULL);
	sleep(1);
	for(i=1;i<10;i++){
		pthread_create(&thread_conss[i],NULL,get_non_bloccante_buffer_unitario_pieno_nonConsumaMessaggio,NULL);
		sleep(1);
	}
}

void join_threadsProducers(){
	int i;
	for(i=0;i<10;i++){
		pthread_join(thread_prods[i],NULL);
	}
}

void join_threadsConsumers(){
	int i;
	for(i=0;i<10;i++){
		pthread_join(thread_conss[i],NULL);
	}
}

void create_concurrent_producers_blocking_oneMessage_buffer_generico_vuoto(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_prods[i],NULL,put_bloccante_buffer_generico_vuoto_messaggio_singolo,(void*)i);
		sleep(1);
	}
}

void create_concurrent_consumers_blocking_oneMessage_buffer_generico_vuoto(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_conss[i],NULL,get_bloccante_buffer_generico_vuoto_messaggio_singolo,(void*) i);
		sleep(1);
	}
}

void create_concurrent_producers_non_blocking_oneMessage_buffer_generico_vuoto(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_prods[i],NULL,put_non_bloccante_buffer_generico_vuoto_messaggio_singolo,(void*)i);
		sleep(1);
	}
}

void create_concurrent_consumers_non_blocking_oneMessage_buffer_generico_vuoto(){
	int i;
	for(i=0;i<10;i++){
		pthread_create(&thread_prods[i],NULL,get_non_bloccante_buffer_generico_vuoto_messaggio_singolo,(void*)i);
		sleep(1);
	}
}

void create_concurrent_producers_blocking_twoMessage(){
	int i;
	for(i=0;i<5;i++){
		pthread_create(&thread_prods[i],NULL,put_bloccante_buffer_generico_vuoto_messaggioDoppio,(void*)i);
		sleep(2);
	}
}

void create_concurrent_producers_non_blocking_twoMessage(){
	int i;
	for(i=0;i<5;i++){
		pthread_create(&thread_prods[i],NULL,put_non_bloccante_buffer_generico_vuoto_messaggioDoppio,(void*)i);
		sleep(2);
	}
}

void join_5threadsProducers(){
	int i;
	for(i=0;i<5;i++){
		pthread_join(thread_prods[i],NULL);
	}
}

void test_put_bloccante_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);
	msg_t* expected_msg = msg_init((void*)stringa_test);
	msg_t* inserted_msg= put_bloccante(buffer_unitario_vuoto,expected_msg);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg,expected_msg);
	CU_ASSERT_PTR_EQUAL(inserted_msg->content,expected_msg->content);
	buffer_destroy(buffer_unitario_vuoto);
}

void test_put_non_bloccante_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);
	msg_t* expected_msg = msg_init((void*)stringa_test);
	msg_t* inserted_msg= put_non_bloccante(buffer_unitario_vuoto,expected_msg);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg,expected_msg);
	CU_ASSERT_PTR_EQUAL(inserted_msg->content,expected_msg->content);
	buffer_destroy(buffer_unitario_vuoto);
}

void test_get_bloccante_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,0);
	buffer_destroy(buffer_unitario_vuoto);
}

void test_get_non_bloccante_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);
	msg_t* retrived_msg = get_non_bloccante(buffer_unitario_vuoto);
	CU_ASSERT_PTR_NULL(retrived_msg);
	buffer_destroy(buffer_unitario_vuoto);
}

void test_putAndGet_bloccante_concorrente_1Produttore1Consumatore_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);
	create_producerAndConsumer_blocking();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,0);
	join_producerAndConsumer();
	buffer_destroy(buffer_unitario_vuoto);	
}

void test_putAndGet_non_bloccante_concorrente_1Produttore1Consumatore_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);
	create_producerAndConsumer_non_blocking();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,0);
	join_producerAndConsumer();
	buffer_destroy(buffer_unitario_vuoto);	
}

void test_getAndPut_bloccante_concorrente_1Consumatore1Produttore_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);
	create_consumerAndProducer_blocking();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,0);
	join_producerAndConsumer();
	buffer_destroy(buffer_unitario_vuoto);	
}

void test_getAndPut_non_bloccante_concorrente_1Consumatore1Produttore_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);
	create_consumerAndProducer_non_blocking();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,1);
	join_producerAndConsumer();
	buffer_destroy(buffer_unitario_vuoto);	
}

void test_putAndGet_bloccante_concorrente_10Produttori10Consumatori_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);	
	create_concurrent_producers_blocking_oneMessage();
	create_concurrent_consumers_blocking_oneMessage();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,0);
	join_threads_oneMessage();
	buffer_destroy(buffer_unitario_vuoto);
}

void test_putAndGet_non_bloccante_concorrente_10Produttori10Consumatori_buffer_unitario_vuoto(){
	buffer_unitario_vuoto=buffer_init(1);	
	create_concurrent_producers_non_blocking_oneMessage();
	create_concurrent_consumers_non_blocking_oneMessage();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,0);
	join_threads_oneMessage();
	buffer_destroy(buffer_unitario_vuoto);
}

void test_put_bloccante_concorrente_10Produttori_buffer_unitario_vuoto() {
	buffer_unitario_vuoto=buffer_init(1);	
	create_concurrent_producers_blocking_onlyOneMessage();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,1);
	join_threadsProducers();
	buffer_destroy(buffer_unitario_vuoto);
}

void test_put_non_bloccante_concorrente_10Produttori_buffer_unitario_vuoto() {
	buffer_unitario_vuoto=buffer_init(1);	
	create_concurrent_producers_non_blocking_onlyOneMessage();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,1);
	join_threadsProducers();
	buffer_destroy(buffer_unitario_vuoto);
}

void test_get_bloccante_buffer_unitario_pieno(){
	buffer_unitario_pieno=buffer_init(1);
	buffer_unitario_pieno_fixture();
	msg_t* retrived_msg = get_bloccante(buffer_unitario_pieno);
	CU_ASSERT_PTR_NOT_NULL(retrived_msg);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringa_test);
	buffer_destroy(buffer_unitario_pieno);
}

void test_get_non_bloccante_buffer_unitario_pieno(){
	buffer_unitario_pieno=buffer_init(1);
	buffer_unitario_pieno_fixture();
	msg_t* retrived_msg = get_non_bloccante(buffer_unitario_pieno);
	CU_ASSERT_PTR_NOT_NULL(retrived_msg);
	CU_ASSERT_PTR_EQUAL(retrived_msg->content,stringa_test);
	buffer_destroy(buffer_unitario_pieno);
}

void test_put_bloccante_buffer_unitario_pieno(){
	buffer_unitario_pieno=buffer_init(1);
	buffer_unitario_pieno_fixture();
	msg_t* expected_msg = msg_init((void*)stringa_errore);
	CU_ASSERT_EQUAL(buffer_unitario_pieno->k,buffer_unitario_pieno->size);
	CU_ASSERT_PTR_NOT_EQUAL(buffer_unitario_pieno->queue[buffer_unitario_pieno->D].content,expected_msg->content);
	buffer_destroy(buffer_unitario_vuoto);
}

void test_put_non_bloccante_buffer_unitario_pieno(){
	buffer_unitario_pieno=buffer_init(1);
	buffer_unitario_pieno_fixture();
	msg_t* expected_msg = msg_init((void*)stringa_errore);
	msg_t* inserted_msg= put_non_bloccante(buffer_unitario_pieno,expected_msg);
	CU_ASSERT_PTR_NOT_EQUAL(inserted_msg,expected_msg);
	CU_ASSERT_PTR_EQUAL(inserted_msg,BUFFER_ERROR);
	buffer_destroy(buffer_unitario_vuoto);
}

void test_get_bloccante_concorrente_10Consumatori_buffer_unitario_pieno(){
	buffer_unitario_pieno=buffer_init(1);
	buffer_unitario_pieno_fixture();
	create_concurrent_consumers_blocking_onlyOneMessage();
	join_threadsConsumers();
	buffer_destroy(buffer_unitario_pieno);
}

void test_get_non_bloccante_concorrente_10Consumatori_buffer_unitario_pieno(){
	buffer_unitario_pieno=buffer_init(1);
	buffer_unitario_pieno_fixture();
	create_concurrent_consumers_non_blocking_onlyOneMessage();
	join_threadsConsumers();
	buffer_destroy(buffer_unitario_pieno);
}

void test_put_bloccante_concorrente_10Produttori_buffer_generico_vuoto(){
	buffer_generico_vuoto=buffer_init(15);
	create_concurrent_producers_blocking_oneMessage_buffer_generico_vuoto();
	CU_ASSERT_EQUAL(buffer_generico_vuoto->k,10);
	join_threadsProducers();
	buffer_destroy(buffer_generico_vuoto);
}	

void test_put_non_bloccante_concorrente_10Produttori_buffer_generico_vuoto(){
	buffer_generico_vuoto=buffer_init(15);
	create_concurrent_producers_non_blocking_oneMessage_buffer_generico_vuoto();
	CU_ASSERT_EQUAL(buffer_generico_vuoto->k,10);
	join_threadsProducers();
	buffer_destroy(buffer_generico_vuoto);
}

void test_put_bloccante_concorrente_5ProduttoriDoppioMessaggio_buffer_generico_vuoto(){
	buffer_generico_vuoto=buffer_init(15);
	create_concurrent_producers_blocking_twoMessage();
	CU_ASSERT_EQUAL(buffer_generico_vuoto->k,10);
	join_5threadsProducers();
	buffer_destroy(buffer_generico_vuoto);
}

void test_put_non_bloccante_concorrente_5ProduttoriDoppioMessaggio_buffer_generico_vuoto(){
	buffer_generico_vuoto=buffer_init(15);
	create_concurrent_producers_non_blocking_twoMessage();
	CU_ASSERT_EQUAL(buffer_generico_vuoto->k,10);
	join_5threadsProducers();
	buffer_destroy(buffer_generico_vuoto);
}

void test_putAndGet_bloccante_concorrente_10Produttori10Consumatori_buffer_generico_vuoto(){
	buffer_generico_vuoto=buffer_init(15);
	create_concurrent_producers_blocking_oneMessage_buffer_generico_vuoto();
	create_concurrent_consumers_blocking_oneMessage_buffer_generico_vuoto();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,0);
	join_producerAndConsumer();
	buffer_destroy(buffer_generico_vuoto);
}

void test_putAndGet_non_bloccante_concorrente_10Produttori10Consumatori_buffer_generico_vuoto(){
	buffer_generico_vuoto=buffer_init(15);
	create_concurrent_producers_non_blocking_oneMessage_buffer_generico_vuoto();
	create_concurrent_consumers_non_blocking_oneMessage_buffer_generico_vuoto();
	CU_ASSERT_EQUAL(buffer_unitario_vuoto->k,0);
	join_producerAndConsumer();
	buffer_destroy(buffer_generico_vuoto);
}

void test_get_bloccante_concorrente_10Consumatori_buffer_generico_pieno(){
	buffer_generico_pieno=buffer_init(10);
	buffer_generico_pieno_fixture();
	create_concurrent_consumers_blocking_oneMessage_buffer_generico_pieno();
	CU_ASSERT_EQUAL(buffer_generico_pieno->k,0);
	join_threadsConsumers();
	buffer_destroy(buffer_generico_pieno);

}

void test_get_non_bloccante_concorrente_10Consumatori_buffer_generico_pieno(){
	buffer_generico_pieno=buffer_init(10);
	buffer_generico_pieno_fixture();
	create_concurrent_consumers_non_blocking_oneMessage_buffer_generico_pieno();
	CU_ASSERT_EQUAL(buffer_generico_pieno->k,0);
	join_threadsConsumers();
	buffer_destroy(buffer_generico_pieno);
}

void test_put_bloccante_concorrente_10Produttori_buffer_generico_pieno(){
	buffer_generico_pieno=buffer_init(10);
	buffer_generico_pieno_fixture();
	create_concurrent_producers_blocking_oneMessage_buffer_generico_pieno();
	CU_ASSERT_EQUAL(buffer_generico_pieno->k,10);
	join_threadsProducers();
	buffer_destroy(buffer_generico_pieno);
}

void test_put_non_bloccante_concorrente_10Produttori_buffer_generico_pieno(){
	buffer_generico_pieno=buffer_init(10);
	buffer_generico_pieno_fixture();
	create_concurrent_producers_non_blocking_oneMessage_buffer_generico_pieno();
	CU_ASSERT_EQUAL(buffer_generico_pieno->k,10);
	join_threadsProducers();
	buffer_destroy(buffer_generico_pieno);
}
