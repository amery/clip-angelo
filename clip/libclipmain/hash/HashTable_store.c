int
HashTable_store(HashTable * self, void *item, long hash_val)
{
   int Size = self->size;

   int Num = self->num;

   int bestspot, i;

   unsigned long h, hashval = (unsigned long) hash_val;

   if (NEED_GROW(Num, Size))
   {
      resize(self, 0);
      Size = self->size;
      Num = self->num;
   }
   bestspot = Size;
   h = hashval % Size;
   for (i = 0; i <= Size; ++i)
   {
      /* if (self->status[h] == DELETEDCELL) { if (bestspot >= Size) bestspot
         = h; } else if (self->status[h] == EMPTYCELL) */
      if (self->status[h] != VALIDCELL)
      {
	 if (bestspot >= Size)
	    bestspot = h;
	 self->items[bestspot] = item;
	 self->hashs[bestspot] = hashval;
	 self->status[bestspot] = VALIDCELL;
	 ++self->num;
	 return true;
      }
      else if (self->hashs[h] == hashval)
      {				/* replace value */
	 self->items[h] = item;
	 return true;
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
   self->items[bestspot] = item;
   self->hashs[bestspot] = hashval;
   self->status[bestspot] = VALIDCELL;

   ++self->num;
   return true;
}
