/*
	BUNZIP2(cStr) --> cResult
*/
int
clip_BUNZIP2(ClipMachine * mp)
{
   int       l, r;

   unsigned int rl;

   char     *s, *rp;

   s = _clip_parcl(mp, 1, &l);

   if (!s || l < 4)
      return EG_ARG;

   rl = read_ulong(s);
   rp = (char *) malloc(rl + 1);

   r = BZPREF(bzBuffToBuffDecompress) (rp, &rl, s + 4, l - 4, 0, 0);

   if (r != BZ_OK)
    {
       free(rp);
       if (r == BZ_DATA_ERROR)
	  return EG_ARG;
       else
	  return EG_MEM;
    }

   _clip_retcn_m(mp, rp, rl);

   return 0;
}
