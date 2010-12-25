int
po_write_entry(FILE * out, PoEntry * ep, int dupflag)
{
   write_po_header(out, ep);

   if ((!ep->msg || !ep->msg[0]) && (!ep->translated || !ep->translated[0]) && !ep->nplural)
   {
      return 0;
   }

   fprintf(out, "msgid \"");
   if (put_str(out, ep->msg))
      return -1;
   fprintf(out, "\"\n");

   if (ep->msg_plural)
   {
      int i;

      fprintf(out, "msgid_plural \"");
      if (put_str(out, ep->msg_plural))
	 return -1;
      fprintf(out, "\"\n");

      for (i = 0; i < ep->nplural; i++)
      {
	 fprintf(out, "msgstr[%d] \"", i);

	 if (ep->plural[i])
	 {
	    if (put_str(out, ep->plural[i]))
	       return -1;
	 }
	 else if (dupflag)
	 {
	    if (put_str(out, ep->msg))
	       return -1;
	 }
	 fprintf(out, "\"\n");
      }

      if (i == 0)
      {
	 fprintf(out, "msgstr[0] \"");
	 if (dupflag)
	 {
	    if (put_str(out, ep->msg))
	       return -1;
	 }
	 fprintf(out, "\"\n");
	 i++;
      }

      if (i == 1)
      {
	 fprintf(out, "msgstr[1] \"");
	 if (dupflag)
	 {
	    if (put_str(out, ep->msg_plural))
	       return -1;
	 }
	 fprintf(out, "\"\n");
      }

   }
   else
   {
      fprintf(out, "msgstr \"");
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
      fprintf(out, "\"\n");
   }

   return 0;
}
