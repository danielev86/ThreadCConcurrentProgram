#include "main.h"
#include <pthread.h>
#include <stdio.h>




void create_requester(pthread_t requester[]){
	int i;
	for(i=0;i<6;i++){
		msg_t* message=msg_init((void*)i);
		pthread_create(&requester[i],NULL,accepter_buffer_insert,(void*)&message);
		sleep(1);
	}
}

void join_requester(pthread_t requester[]){
	int i;
	for(i=0;i<6;i++){
		pthread_join(requester[i],NULL);
	}
}


void *accepter_submiter(void* name){
	char* nome=(char*)name;
		
	submitRequest(accepter_buffer, nome);
	pthread_exit(NULL);
}


void fde_main(){
	pthread_t provider;
	pthread_t accepter;
	pthread_t dispatcher[2];
	pthread_t requester[6];
	int dim=10;
	
	printf("Inizializzazione dei buffer (dimensione=10)\n");
	provider_buffer_initialize(dim);
	accepter_buffer_initialize(dim);
	reader_list_initialize();
	sleep(1);

	printf("Invio delle richieste\n");
	int i;
	create_requester(requester);
	join_requester(requester);
	i=0;
	printf("Partenza dell'accepter\n");
		switch(i){
			case 0:
				pthread_create(&accepter,NULL,accepter_submiter,"Reader 1");
				pthread_join(accepter,NULL);

			case 1:
				pthread_create(&accepter,NULL,accepter_submiter,"Reader 2");
				pthread_join(accepter,NULL);

			case 2:
				pthread_create(&accepter,NULL,accepter_submiter,"Reader 3");
				pthread_join(accepter,NULL);

			case 3:
				pthread_create(&accepter,NULL,accepter_submiter,"Reader 4");
				pthread_join(accepter,NULL);
				sleep(1);

			case 4:
				pthread_create(&accepter,NULL,accepter_submiter,"Reader 5");
				pthread_join(accepter,NULL);

			case 5:
				pthread_create(&accepter,NULL,accepter_submiter,"Reader 6");
				pthread_join(accepter,NULL);
			case 6: 
				break;
			i++;
		}	
		

	
	sequence.size=9;
	sequence.messages[0]=msg_init((void*)(1));
	sequence.messages[1]=msg_init((void*)(2));
	sequence.messages[2]=msg_init((void*)(3));
	sequence.messages[3]=msg_init((void*)(4));
	sequence.messages[4]=msg_init((void*)(5));
	sequence.messages[5]=msg_init((void*)(6));
	sequence.messages[6]=msg_init((void*)(7));
	sequence.messages[7]=msg_init((void*)(8));
	printf("Partenza provider\n");
	pthread_create(&provider, NULL,create_message_sequence,(void*)&sequence);
	
	printf("Partenza del dispatcher\n");
	sleep(1);
	pthread_create(&dispatcher[0],NULL,dispatch_message,10);
	sleep(1);

	pthread_join(provider,NULL);
	pthread_join(dispatcher[0],NULL);
	printf("Chiusura dei flussi\n");
	printf("cancellazione dei buffer\n");
	provider_buffer_destroy();
	accepter_buffer_destroy();	
}
