static void
end_pl_index(PoEntry * ep, int *pl_index, Buf * msgstr)
{
   int ind;

   ind = *pl_index;
   if (ind < 0)
      return;

   if (ind >= ep->nplural)
   {
      int i;

      ep->plural = (char **) realloc(ep->plural, sizeof(char *) * (ind + 1));

      for (i = ep->nplural; i <= ind; i++)
	 ep->plural[i] = 0;

      ep->nplural = ind + 1;
   }

   if (ep->plural[ind])
      free(ep->plural[ind]);

	ep->plural[ind] = msgstr->ptr_of_Buf;

   memset(msgstr, 0, sizeof(Buf));
   *pl_index = -1;
}
