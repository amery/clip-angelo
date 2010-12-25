static void
resize(HashTable * self, int newsize)
{
   int oldsize = self->size, i;

   void **olditems;

   unsigned long *oldhashs;

   char *oldstatus;

   if (newsize <= self->num)
   {
      newsize = FIRST_SIZE;
      while (NEED_GROW(self->num, newsize))
	 newsize *= 2;
   }
   self->size = newsize;

   olditems = self->items;
   oldstatus = self->status;
   oldhashs = self->hashs;

   self->items = NEWVECT(void *, newsize);

   self->status = NEWVECT(char, newsize);

   self->hashs = NEWVECT(unsigned long, newsize);

   for (i = 0; i < newsize; i++)
      self->status[i] = EMPTYCELL;

   self->num = 0;
   for (i = 0; i < oldsize; i++)
   {
      if (oldstatus[i] == VALIDCELL)
	 HashTable_store(self, olditems[i], oldhashs[i]);
   }

   free(olditems);
   free(oldstatus);
   free(oldhashs);
}
