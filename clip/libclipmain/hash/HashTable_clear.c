void
HashTable_clear(HashTable * self)
{
   int i;

   self->num = 0;
   for (i = 0; i < self->size; i++)
      self->status[i] = EMPTYCELL;
}
