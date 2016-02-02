#include "poison_pill.h"
#include <stdlib.h>



const msg_t POISON_PILL_MSG = {
NULL,
msg_init_pill,
msg_destroy_pill,
msg_copy_pill
};

msg_t* msg_init_pill(void *content){
	return POISON_PILL;
}

msg_t* msg_copy_pill(msg_t* message){ 
	return POISON_PILL;
}

void msg_destroy_pill(msg_t *message) { 
	/* do nothing */
}
