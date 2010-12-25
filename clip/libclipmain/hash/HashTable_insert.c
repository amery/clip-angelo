int
HashTable_insert(HashTable * self, void *item, long hash)
{
   if (HashTable_fetch(self, hash))
      return 0;
   return HashTable_store(self, item, hash);
}
