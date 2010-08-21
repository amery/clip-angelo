void
init_Coll(Coll * coll, void (*Free) (void *), int (*compare) ( /* void*,void* */ ))
{
   coll->count_of_Coll = 0;
	coll->size_of_Coll = FIRST_SIZE;
	coll->items_of_Coll = (void **) malloc(sizeof(void *) * FIRST_SIZE);

	coll->_free = Free;
	coll->compare = compare;
	coll->duplicates_of_Coll = 0;
}
