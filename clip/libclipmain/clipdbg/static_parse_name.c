static void
parse_name(const char *name, long *hashp, long **dimp, int *ndimp)
{
   const char *s;

   int l;

   long val;

   if (!memcmp(name, "::", 2))
   {
      *hashp = _clip_casehashbytes(0, "__SELF__", 8);
      l = strcspn(name + 1, "[]:");
      s = name + 1 + l;
   }
   else
   {
      l = strcspn(name, "[]:");
      *hashp = _clip_casehashbytes(0, name, l);
      s = name + l;
   }
   for (;;)
   {
      l = strspn(s, "[]:");
      s += l;
      l = strcspn(s, "[]:");
      if (!l)
	 break;
      *dimp = (long *) realloc(*dimp, sizeof(long) * (1 + *ndimp));

      if (is_dig(s, l))
	 val = atoi(s) - 1;
      else
	 val = _clip_casehashbytes(0, s, l);
      (*dimp)[*ndimp] = val;
      (*ndimp)++;
      s += l;
   }
}
