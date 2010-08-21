const char *
get_HashName(long hash)
{
   int no;

   char *entry;

   if (!search_Coll(&hashNames, &hash, &no))
      return 0;
	entry = (char *) hashNames.items_of_Coll[no];
   return entry + sizeof(long);
}
