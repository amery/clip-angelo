void
delete_HashTable(HashTable * self)
{
   free(self->items);
   free(self->hashs);
   free(self->status);
   free(self);
}
