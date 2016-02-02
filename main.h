#ifndef MAIN_H_
#define MAIN_H_
#include <pthread.h>
#include "provider.h"
#include "accepter.h"
#include "accepter_buffer.h"
#include "reader_list.h"
#include "dispatcher.h"
struct messaggi sequence;


void create_requester(pthread_t requester[]);
void join_requester(pthread_t requester[]);
void fde_main();

#endif
