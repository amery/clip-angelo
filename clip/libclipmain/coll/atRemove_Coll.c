void
atRemove_Coll(Coll * coll, int index)
{
	coll->count_of_Coll--;
	memmove(coll->items_of_Coll + index, coll->items_of_Coll + index + 1, (coll->count_of_Coll - index) * sizeof(void *));
}
