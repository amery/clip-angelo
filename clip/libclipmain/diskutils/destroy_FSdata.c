/* ���������� ������ FSdata */
void
destroy_FSdata(void *item)
{
   FSdata *d = (FSdata *) item;

   int i = 0;

   if (d == NULL)
      return;

   if (d->fname)
   {
      while (d->fname[i])
	 free(d->fname[i++]);
      free(d->fname);
   }

   if (d->st)
   {
      free(d->st);
   }

   free(d);
}
