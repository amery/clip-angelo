static int
_sql_date_compare(void *op, void *key1, void *key2, int *uniq)
{
   int       k1 = *(int *) key1;

   int       k2 = *(int *) key2;

   long      r;

   r = *(long *) ((char *) key1 + sizeof(int)) - *(long *) ((char *) key2 + sizeof(int));
   if (!r && k1 > 0)
      r = k1 - k2;
   return (int) r;
}
