/**Header che rappresenta un messaggio di tipo generico **/



#ifndef MESSAGE_H_
#define MESSAGE_H_

/** Tipo di dato messaggio **/
typedef struct msg {
	void* content; //contenuto generico del messaggio
	struct msg* (*msg_init)(void*); //creazione messaggio
	void (*msg_destroy)(struct msg*); // deallocazione messaggio
	struct msg* (*msg_copy)(struct msg*); //costruttore di copia
} msg_t;

/** Signature delle funzioni del tipo messaggio **/
msg_t* msg_init(void* content);
void msg_destroy(msg_t* msg);
msg_t* msg_copy(msg_t* msg);

#endif
