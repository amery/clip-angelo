static int
compkeys(void *ap, void *bp)
{
   int cmp;

   Keytab *a = (Keytab *) ap;

   Keytab *b = (Keytab *) bp;

   if (!a->str)
   {
      if (!b->str)
	 return (0);
      return (1);
   }
   if (!b->str)
      return (-1);
   cmp = strcmp(a->str, b->str);
   return (cmp);
}
