#include <CUnit/CUnit.h>
#include "reader_listDomainAssertion.h"



void ASSERT_SINGLETON_READERLIST(list_t *list, reader_t* expected) {
  CU_ASSERT_PTR_EQUAL(list->tail, list->head);
  CU_ASSERT_PTR_EQUAL(list->tail->payload, expected);
  CU_ASSERT_PTR_NULL(list->tail->next);
}

void ASSERT_SINGLETON_READERLIST_FALSE(list_t *list, reader_t* expected) {
  CU_ASSERT_PTR_EQUAL(list->tail, list->head);
  CU_ASSERT_PTR_NOT_EQUAL(list->tail->payload, expected);
  CU_ASSERT_PTR_NULL(list->tail->next);
}

void ASSERT_READER_INSERTED(list_t* reader_list){
	iterator_t* iterator=iterator_init(reader_list);
	CU_ASSERT_TRUE(hasNext(iterator));
	reader_t* reader= (reader_t*)next(iterator);
	ASSERT_SINGLETON_READERLIST(reader_list, reader);
}

void ASSERT_READER_NOT_INSERTED(list_t* reader_list){
	iterator_t* iterator=iterator_init(reader_list);
	CU_ASSERT_TRUE(hasNext(iterator));
	reader_t* reader= (reader_t*)next(iterator);
	ASSERT_SINGLETON_READERLIST_FALSE(reader_list, reader);
}
