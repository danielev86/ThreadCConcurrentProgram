/** Implementazione delle funzioni di accesso al buffer **/

#include "buffer.h"
#include <stdlib.h>



/** Creazione di un buffer vuoto di dimensione massima nota **/

buffer_t* buffer_init(unsigned int maxsize){
	buffer_t* buffer = (buffer_t*)malloc(sizeof(buffer_t));
	buffer->queue=(msg_t*)malloc(maxsize*sizeof(msg_t));
	buffer->T=buffer->D=0;
	pthread_mutex_init(&(buffer->uso_t), NULL);
	pthread_mutex_init(&(buffer->uso_d), NULL);
	pthread_cond_init(&(buffer->non_pieno),NULL);
	pthread_cond_init(&(buffer->non_vuoto),NULL);
	buffer->k=0;
	buffer->size=maxsize;
	return buffer;
}

/** Deallocazione di un buffer **/

void buffer_destroy(buffer_t* buffer){
	pthread_cond_destroy(&(buffer->non_pieno));
	pthread_cond_destroy(&(buffer->non_vuoto));
	pthread_mutex_destroy(&(buffer->uso_t));
	pthread_mutex_destroy(&(buffer->uso_d));		
	free(buffer->queue);
	free(buffer);
}

/** Inserimento bloccante: sospende se pieno, quindi
 *  effettua l'inserimento non appena si libera dello spazio.
 *  Restituisce il messaggio inserito. N.B.: msg!=NULL **/

msg_t* put_bloccante(buffer_t* buffer, msg_t* msg){
	msg_t* temp = msg_copy(msg);
	pthread_mutex_lock(&(buffer->uso_d));
	while(buffer->k==buffer->size){
		pthread_cond_wait(&(buffer->non_pieno),&(buffer->uso_d));
	}
	buffer->queue[buffer->D]=*temp;
	buffer->D=(buffer->D+1)%(buffer->size);
	buffer->k++;
	pthread_cond_signal(&(buffer->non_vuoto));
	pthread_mutex_unlock(&(buffer->uso_d));	
	return temp;
}

/** Inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
 *  altrimenti effettua l'insermento e restituisce il messaggio inserito.
 *  N.B.: msg!=NULL **/

msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg){
	msg_t* temp= msg_copy(msg);
	pthread_mutex_lock(&(buffer->uso_d));
	if(buffer->k==buffer->size){
		pthread_mutex_unlock(&(buffer->uso_d));
		return BUFFER_ERROR;
	}
	buffer->queue[buffer->D]=*temp;
	buffer->D=(buffer->D+1)%(buffer->size);
	buffer->k++;
	pthread_cond_signal(&(buffer->non_vuoto));
	pthread_mutex_unlock(&(buffer->uso_d));
	return temp;
}

/** Estrazione bloccante: sospende se vuoto, quindi restituisce
 *  il valore estratto non appena disponibile. **/

msg_t* get_bloccante(buffer_t* buffer){
	msg_t* msg= NULL;	
	pthread_mutex_lock(&(buffer->uso_t));
	while(buffer->k==0){
		pthread_cond_wait(&(buffer->non_vuoto),&(buffer->uso_t));
	}
	msg=msg_copy(&(buffer->queue[buffer->T]));
	buffer->queue[buffer->T].content=NULL;	//Cancello il contenuto, lasciando attivo il puntatore per poi sovrascriverlo
	buffer->T=(buffer->T+1)%(buffer->size);
	buffer->k--;
	pthread_cond_signal(&(buffer->non_pieno));
	pthread_mutex_unlock(&(buffer->uso_t));

	return msg;
}

/** Estrazione non bloccante: restituisce BUFFER_ERROR se vuoto,
 *  altrimenti restituisce il valore estratto. **/

msg_t* get_non_bloccante(buffer_t* buffer){
	msg_t* msg= NULL;	
	pthread_mutex_lock(&(buffer->uso_t));
	if(buffer->k==0){
		pthread_mutex_unlock(&(buffer->uso_t));
		return BUFFER_ERROR;
	}
	msg=msg_copy(&(buffer->queue[buffer->T]));
	buffer->queue[buffer->T].content=NULL;
	buffer->T=(buffer->T+1)%(buffer->size);
	buffer->k--;
	pthread_cond_signal(&(buffer->non_pieno));
	pthread_mutex_unlock(&(buffer->uso_t));
	return msg;
}
