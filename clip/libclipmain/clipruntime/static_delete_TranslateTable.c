static void
delete_TranslateTable(void *p)
{
   TranslateTable *tp = (TranslateTable *) p;

   if (!tp)
      return;

   free(tp->src);
   free(tp->dst);
   if (tp->pg)
      free(tp->pg);

   free(tp);
}
