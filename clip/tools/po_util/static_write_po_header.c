static void
write_po_header(FILE * out, PoEntry * ep)
{
   PoEntry *pp = ep;

   int i;

   for (i = 0; i < ep->ncomments; i++)
      fprintf(out, "%s\n", ep->comments[i]);

   if (!ep->file)
      return;

   for (;;)
   {
      int i;

      fprintf(out, "\n#:");

      for (i = 0; i < 5; ++i)
      {
	 fprintf(out, " %s:%d", pp->file, pp->line);
	 if (pp->next)
	    pp = pp->next;
	 else
	    goto ret;
      }
   }

 ret:
   if (!ep->ncomments)
      fprintf(out, "\n");
}
