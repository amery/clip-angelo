static int
_sql_num_compare(void *op, void *key1, void *key2, int *uniq)
{
   int k1 = *(int *) key1;

   int k2 = *(int *) key2;

   double d;

   int r = 0;

   d = *(double *) ((char *) key1 + sizeof(int)) - *(double *) ((char *) key2 + sizeof(int));
   if (d < 0)
      r = -1;
   else if (d > 0)
      r = 1;
   if (!r && k1 > 0)
      r = k1 - k2;
   return r;
}
