#include "../message.h"
#include "testMessage.h"
#include "CUnit/CUnit.h"




char* stringa_test;

int message_before(){
	stringa_test="Test";
	return 0;
}

int message_after(){
	return 0;
}

msg_t* message_fixture(char stringa[]){
	return msg_init((void*) stringa_test);
	}

void test_msg_init (void) {	
	msg_t* test=msg_init((void*) stringa_test);
	CU_ASSERT_PTR_NOT_NULL(test);
	CU_ASSERT_PTR_EQUAL(test->content, stringa_test);
}

void test_msg_copy(void) {
	msg_t* test= message_fixture(stringa_test);
	msg_t* copia = msg_copy(test);
	CU_ASSERT_PTR_NOT_EQUAL(copia,test);
	CU_ASSERT_PTR_EQUAL(copia->content, stringa_test);
}

