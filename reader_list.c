#include "reader_list.h"



/*int reader_list_processingTimeMean(){
	pthread_mutex_lock(&(reader_list_mutex));
	iterator=iterator_init(reader_list);
	int somma=0;
	while(hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		somma+=reader->processing_time;
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&(reader_list_mutex));
	int n=reader_list_size();
	double mean=somma/n;
	return mean;
}*/

void reader_list_initialize(){
	reader_list=list_init();
	pthread_mutex_init(&(reader_list_mutex),NULL);	
}

void reader_list_destroy(){
	pthread_mutex_destroy(&(reader_list_mutex));
	list_destroy(reader_list);
}

int reader_list_size (){
	pthread_mutex_lock(&(reader_list_mutex));
	int dim=size(reader_list);
	pthread_mutex_unlock(&(reader_list_mutex));
	return dim;
}

int reader_list_isEmpty(){
	pthread_mutex_lock(&(reader_list_mutex));
	int vuota=isEmpty(reader_list);
	pthread_mutex_unlock(&(reader_list_mutex));
	return vuota;
}

void reader_list_addReader(void* reader){
	pthread_mutex_lock(&(reader_list_mutex));
	addElement(reader_list, reader);
	pthread_mutex_unlock(&(reader_list_mutex));
}

int reader_list_removeReader(void* reader){
	pthread_mutex_lock(&(reader_list_mutex));
	int deleted= removeElement(reader_list, reader);
	pthread_mutex_unlock(&(reader_list_mutex));
	return deleted;
}

void reader_list_kill(){
	pthread_mutex_lock(&(reader_list_mutex));
	iterator=iterator_init(reader_list);
	while(hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		reader_buffer_insert(reader->buffer,(msg_t*)&POISON_PILL_MSG);
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&(reader_list_mutex));
}

void reader_list_broadcast(msg_t* message){
	pthread_mutex_lock(&(reader_list_mutex));
	iterator=iterator_init(reader_list);
	while(hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		reader_buffer_insert(reader->buffer,message);
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&(reader_list_mutex));
}

/*reader_t* reader_list_analyze(){
	//pthread_mutex_lock(&(reader_list_mutex));
	double mean=reader_list_processingTimeMean();
	pthread_mutex_lock(&(reader_list_mutex));
	iterator=iterator_init(reader_list);
	if(mean>=2){
		while(hasNext(iterator)){
			reader_t* reader=(reader_t*)next(iterator);
			if(reader->processing_time>mean){
				pthread_mutex_unlock(&(reader_list_mutex));
				return reader;
			}
		}
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&(reader_list_mutex));
	return NULL;
}*/

void reader_list_delete_reader(int processamento){
	pthread_mutex_lock(&(reader_list_mutex));
	iterator=iterator_init(reader_list);
	int i;
	while(hasNext(iterator)){
		reader_t* reader=(reader_t*)next(iterator);
		if(reader->processing_time>=processamento){
			for(i=reader_buffer_numberOfMessage(reader->buffer);i<reader_buffer_size(reader->buffer);i++){
							reader_buffer_insert(reader->buffer,(msg_t*)&POISON_PILL_MSG);
			}
		}

	}
	pthread_mutex_unlock(&(reader_list_mutex));


}


