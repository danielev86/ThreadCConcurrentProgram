/**Headed per la definizione del buffer **/

#ifndef BUFFER_H_
#define BUFFER_H_



/** Definzione di una costante per rappresentare l'errore nell'accesso al buffer**/
#define BUFFER_ERROR (msg_t*) NULL

#include "message.h"
#include <pthread.h>

/** Definizione del tipo buffer **/

typedef struct buffer {
	msg_t* queue; //il buffer vero e proprio
	int T; //puntatore per le estrazioni
	int D; //puntatore per gli inserimenti
	int size; //Dimensione buffer
	int k; //Numero messaggi inseriti
	pthread_cond_t non_vuoto; //variabile condizione per indicare il buffer non vuoto
	pthread_cond_t non_pieno; //variabile condizione per indicare il buffer non pieno
	pthread_mutex_t uso_t; //mutex per l'accesso all'indice delle estrazioni
	pthread_mutex_t uso_d; //mutex per l'accesso all'indice degli inserimenti
} buffer_t;

/** Signature delle funzioni per il buffer **/

buffer_t* buffer_init(unsigned int maxsize);
void buffer_destroy(buffer_t* buffer);
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg);
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg);
msg_t* get_bloccante(buffer_t* buffer);
msg_t* get_non_bloccante(buffer_t* buffer);


#endif
