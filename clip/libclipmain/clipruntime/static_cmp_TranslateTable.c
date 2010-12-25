static int
cmp_TranslateTable(void *p1, void *p2)
{
   TranslateTable *tp1 = (TranslateTable *) p1;

   TranslateTable *tp2 = (TranslateTable *) p2;

   int r;

   r = strcasecmp(tp1->src, tp2->src);
   if (r)
      return r;

   return strcasecmp(tp1->dst, tp2->dst);
}
