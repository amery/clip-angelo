int
po_write_entry_compat(FILE * out, PoEntry * ep, int dupflag)
{
   int i;

   write_po_header(out, ep);

   if ((!ep->msg || !ep->msg[0]) && (!ep->translated || !ep->translated[0]) && !ep->nplural)
   {
      return 0;
   }

   fprintf(out, "msgid \"");
   if (put_str(out, ep->msg))
      return -1;

   if (ep->msg_plural)
   {
      fputc(COMPAT_DELIM, out);
      if (put_str(out, ep->msg_plural))
	 return -1;
   }
   fprintf(out, "\"\n");

   fprintf(out, "msgstr \"");
   if (ep->msg_plural)
   {
      for (i = 0; i < ep->nplural; i++)
      {
	 if (i)
	    fputc(COMPAT_DELIM, out);
	 if (ep->plural[i])
	 {
	    if (put_str(out, ep->plural[i]))
	       return -1;
	 }
	 else if (dupflag)
	 {
	    if (i == 0)
	       put_str(out, ep->msg);
	    else
	       put_str(out, ep->msg_plural);
	 }
      }

      if (i == 0)
      {
	 if (dupflag)
	 {
	    if (put_str(out, ep->msg))
	       return -1;
	 }
	 i++;
      }

      if (i == 1)
      {
	 fputc(COMPAT_DELIM, out);
	 if (dupflag)
	 {
	    if (put_str(out, ep->msg_plural))
	       return -1;
	 }
      }

   }
   else
   {
      if (ep->translated)
      {
	 if (put_str(out, ep->translated))
	    return -1;
      }
      else if (dupflag)
      {
	 if (put_str(out, ep->msg))
	    return -1;
      }
   }

   fprintf(out, "\"\n");
   return 0;
}
