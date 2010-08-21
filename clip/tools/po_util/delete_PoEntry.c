void
delete_PoEntry(void *p)
{
   PoEntry *ep = (PoEntry *) p;

   if (!ep)
      return;

   if (ep->next)
      delete_PoEntry(ep->next);

   if (ep->msg)
      free(ep->msg);
   if (ep->translated)
      free(ep->translated);
   if (ep->file)
      free(ep->file);

   if (ep->plural)
   {
      int i;

      for (i = 0; i < ep->nplural; i++)
	 free(ep->plural[i]);
      free(ep->plural);
   }

   if (ep->comments)
   {
      int i;

      for (i = 0; i < ep->ncomments; i++)
	 free(ep->comments[i]);
      free(ep->comments);
   }

   free(ep);
}
