/*
	Copyright (C) 2001  ITK
	Author   : Paul Lasarev <paul@itk.ru>
	License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <stdlib.h>
#include <ci_clip.h>

/*#include <ci_clip.h>*/
#include <ci_hash.h>

#include <hash/struct__HashTable.h>

static void resize(HashTable * self, int newsize);

static unsigned long doublehashinc(unsigned long h, unsigned long s);

#define true 1
#define false 0
#define null ((void*)0)

#define EMPTYCELL 0
#define VALIDCELL 1
#define DELETEDCELL 2

#define FIRST_SIZE 16
#define NEED_GROW(c,s) ((c)*5/4>=(s))

#define NEW(type) ((type*)malloc(sizeof(type)))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))
#define RESIZE(ptr,type,len) ((type*)realloc(ptr,sizeof(type)*(len)))
#define VAR(type,var,ini) type *var=(type*)(ini)
#define NEWVAR(type,var) type *var=(type*)malloc(sizeof(type))

#include <hash/new_HashTable.c>
#include <hash/delete_HashTable.c>
#include <hash/HashTable_len.c>
#include <hash/HashTable_clear.c>
#include <hash/HashTable_Clear.c>
#include <hash/static_doublehashinc.c>
#include <hash/static_resize.c>
#include <hash/HashTable_insert.c>
#include <hash/HashTable_store.c>
#include <hash/HashTable_fetch.c>
#include <hash/HashTable_remove.c>
#include <hash/HashTable_first.c>
#include <hash/HashTable_next.c>
#include <hash/HashTable_current.c>
