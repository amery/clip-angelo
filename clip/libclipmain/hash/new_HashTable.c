HashTable *
new_HashTable()
{
   NEWVAR(HashTable, tp);
   tp->num = 0;
   tp->size = FIRST_SIZE;
   tp->current = 0;
   tp->items = NEWVECT(void *, FIRST_SIZE);

   tp->hashs = (unsigned long *) NEWVECT(long, FIRST_SIZE);

   tp->status = NEWVECT(char, FIRST_SIZE);

   HashTable_clear(tp);
   return tp;
}
