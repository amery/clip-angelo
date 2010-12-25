void
atFree_Coll(Coll * coll, int pos)
{
   if (coll->_free)
		coll->_free(coll->items_of_Coll[pos]);
   atRemove_Coll(coll, pos);
}
