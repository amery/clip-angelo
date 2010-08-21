/*
	BZIP2(cStr[, nLevel]) --> cResult
*/
int
clip_BZIP2(ClipMachine * mp)
{
   int       l, r;

   unsigned int rl;

   int       level = 9;

   char     *s, *rp, *rp1;

   s = _clip_parcl(mp, 1, &l);

   if (!s)
      return EG_ARG;

   if (_clip_parinfo(mp, 1) == 'N')
      level = _clip_parni(mp, 1);
   if (level < 1 || level > 9)
      level = 9;

   if (l < 10)
      rl = l + 1 + 600;
   else
      rl = l + (l / 10) + 600;
   rp = (char *) malloc(rl + 4);
   rp1 = rp + sizeof(long);

   r = BZPREF(bzBuffToBuffCompress) (rp1, &rl, s, l, level, 0, 0);

   if (r != BZ_OK)
    {
       free(rp);
       return EG_MEM;
    }

   write_ulong(rp, l);

   rp = (char *) realloc(rp, rl + 5);
   rp[rl + 4] = 0;

   _clip_retcn_m(mp, rp, rl + 4);

   return 0;
}

