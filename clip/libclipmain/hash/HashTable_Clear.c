void
HashTable_Clear(HashTable * self)
{
   self->size = FIRST_SIZE;
   self->items = RESIZE(self->items, void *, FIRST_SIZE);

   self->hashs = RESIZE(self->hashs, unsigned long, FIRST_SIZE);

   self->status = RESIZE(self->status, char, FIRST_SIZE);

   HashTable_clear(self);
}
