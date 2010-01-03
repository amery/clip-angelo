static int
_sql_char_compare(void *op, void *key1, void *key2, int *uniq)
{
   SQLORDER *order = (SQLORDER *) op;

   int       k1 = *(int *) key1;

   int       k2 = *(int *) key2;

   int       l, r;

   l = order->len - sizeof(int);
   if (k1 <= 0)
      l = -k1;
   r = sql_cmp(order->loc, (unsigned char *) key1 + sizeof(int), (unsigned char *) key2 + sizeof(int), l);
   if (!r && k1 >= 0)
      r = k1 - k2;
   return r;
}
