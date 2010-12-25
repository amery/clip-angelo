PoEntry *
new_PoEntry(char *msg, char *file, int line, PoEntry * pp)
{
   PoEntry *rp;

   rp = (PoEntry *) calloc(1, sizeof(PoEntry));

   if (msg)
      rp->msg = strdup(msg);
   else
      rp->msg = 0;

   if (file)
      rp->file = strdup(file);
   else
      rp->file = 0;

   rp->line = line;

   rp->next = 0;

   if (pp)
   {
      for (;;)
      {
	 if (pp->next)
	    pp = pp->next;
	 else
	    break;
      }
      pp->next = rp;
   }

   return rp;
}
