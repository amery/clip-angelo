void *
HashTable_remove(HashTable * self, long hash_val)
{
   int Size = self->size;

   unsigned long hashval = (unsigned long) hash_val;

   unsigned long h = hashval % Size;

   int i;

   for (i = 0; i <= Size; ++i)
   {
      if (self->status[h] == EMPTYCELL)
	 return null;
      else if (self->status[h] == VALIDCELL && self->hashs[h] == hashval)
      {
	 self->status[h] = DELETEDCELL;
	 --self->num;
	 return self->items[h];
      }
      if (i == 0)
	 h = (h + doublehashinc(hashval, Size)) % Size;
      else
      {
	 ++h;
	 if (h >= Size)
	    h -= Size;
      }
   }
   return null;
}
