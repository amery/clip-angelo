/*
	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

 */

#ifndef CI_HASH_H
#define CI_HASH_H

typedef struct _HashTable HashTable;

HashTable *new_HashTable();
void delete_HashTable(HashTable * self);

int HashTable_len(HashTable * self);
void HashTable_clear(HashTable * self);
void HashTable_Clear(HashTable * self);	/* decrease table size to initial */
int HashTable_insert(HashTable * self, void *item, long hash);
int HashTable_store(HashTable * self, void *item, long hash);
void *HashTable_fetch(HashTable * self, long hash);
void *HashTable_remove(HashTable * self, long hash);

int HashTable_first(HashTable * self);
int HashTable_next(HashTable * self);
void *HashTable_current(HashTable * self);

#endif
