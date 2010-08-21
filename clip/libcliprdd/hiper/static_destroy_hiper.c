static void
destroy_hiper(void *d)
{
   HIPER *hs = (HIPER *) d;

   if (hs)
   {
      if (hs->path)
	 free(hs->path);
      if (hs->name)
	 free(hs->name);
      free(hs->map);
      if (hs->rmap)
	 free(hs->rmap);
      free(hs);
   }
}
