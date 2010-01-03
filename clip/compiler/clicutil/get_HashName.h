const char *
get_HashName(long hash)
{
   int       no;

   char     *entry;

   if (!search_Coll(&hashNames, &hash, &no))
      return 0;
   entry = (char *) hashNames.items[no];
   return entry + sizeof(long);
}
