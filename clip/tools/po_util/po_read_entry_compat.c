PoEntry *
po_read_entry_compat(FILE * in)
{
   PoEntry *ep;

   char *s;

   char *sp;

   ep = po_read_entry(in);
   if (!ep)
      return 0;

   if (ep->nplural || ep->plural || !ep->msg || ep->msg_plural)
      return ep;

   if (!(s = strchr(ep->msg, COMPAT_DELIM)))
      return ep;

   if ((sp = strchr(s + 1, COMPAT_DELIM)))
      return ep;

   ep->msg_plural = strdup(s + 1);
   *s = 0;

   for (s = ep->translated; s && *s;)
   {
      ep->plural = (char **) realloc(ep->plural, sizeof(char *) * (ep->nplural + 1));
      ep->plural[ep->nplural] = s;
      ep->nplural++;
      s = strchr(s, COMPAT_DELIM);
      if (!s)
	 break;
      *s = 0;
      s++;
   }

   return ep;
}
