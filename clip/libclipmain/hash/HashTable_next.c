int
HashTable_next(HashTable * self)
{
   int i;

   if (self->num < 1)
      return 0;
   for (i = ++self->current; i < self->size; i++)
      if (self->status[i] == VALIDCELL)
      {
	 self->current = i;
	 return true;
      }
   return false;
}
