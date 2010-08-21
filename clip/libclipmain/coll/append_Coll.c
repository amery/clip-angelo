void
append_Coll(Coll * coll, void *item)
{
	atInsert_Coll(coll, item, coll->count_of_Coll);
}
