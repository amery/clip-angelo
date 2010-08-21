void
add_HashName(long hash, const char *name)
{
   int no;

   char *entry;

   if (search_Coll(&hashNames, &hash, &no))
      return;
   no = strlen(name);
   entry = (char *) malloc(sizeof(long) + no + 1);

   *(long *) entry = hash;
   memcpy(entry + sizeof(long), name, no + 1);

   insert_Coll(&hashNames, entry);
}
