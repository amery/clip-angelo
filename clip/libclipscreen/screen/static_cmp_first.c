static int
cmp_first(const void *k1, const void *k2)
{
   char *key = (char *) k1;

   Keytab *el = (Keytab *) k2;

   if (!el->str)
      return -1;
   if (*key == el->str[0])
      return 0;
   else if (*key < el->str[0])
      return -1;
   return 1;
}
