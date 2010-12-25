void *
HashTable_current(HashTable * self)
{
   return self->items[self->current];
}
