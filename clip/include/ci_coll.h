/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI
 */

#ifndef CN_COLL_H
#define CN_COLL_H

#define ARR_ALLOC(type,len)       ((type*)malloc(sizeof(type)*(len)))
#define ARR_REALLOC(type,ptr,len) ((ptr)=(type*)realloc(ptr,sizeof(type)*(len)))
#define ARR_SEARCH(type,ptr,len)  ((type*)bsearch(ptr,len,sizeof(type),compare_##type))
#define ARR_SORT(type,ptr,len)    qsort(ptr,len,sizeof(type),compare_##type)
#include "ci_coll/typedef_struct_Coll.h"

Coll     *new_Coll(void (*_free) (void *), int (*compare) ());

void      delete_Coll(void *coll);

void      init_Coll(Coll * coll, void (*_free) (void *), int (*compare) ());

void      destroy_Coll(Coll * coll);

void      freeAll_Coll(Coll * coll);

void      removeAll_Coll(Coll * coll);

int       indexOf_Coll(Coll * coll, void *item, int *index);

int       insert_Coll(Coll * coll, void *item);

int       Insert_Coll(Coll * coll, void *item, int *index);

int       remove_Coll(Coll * coll, void *item);

int       free_Coll(Coll * coll, void *item);

void      append_Coll(Coll * coll, void *item);

void      prepend_Coll(Coll * coll, void *item);

int       atInsert_Coll(Coll * coll, void *item, int index);

void      atRemove_Coll(Coll * coll, int index);

void      atFree_Coll(Coll * coll, int index);

int       search_Coll(Coll * coll, void *item, int *index);

#endif
