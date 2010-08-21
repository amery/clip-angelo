int
clip_TABEXPAND(ClipMachine * ClipMachineMemory)
{
   int l, rl, cur = 0, nt, nl, pos = 0;

   unsigned char *e, *end, *beg, *ret, cch;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   int tlen = _clip_parni(ClipMachineMemory, 2);

   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 3);

   int ch = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   ch = s == NULL ? ch : *s;
   ch = ch == 0 ? ' ' : ch;
   tlen = tlen <= 0 ? 8 : tlen;
   rl = 0;
   ret = malloc(rl + 1);
   for (e = str, beg = str, end = str + l; e < end; e++, cur++)
   {

      cch = *e;
      if (cch == '\n')
	 cur = -1;
      if (cch == '\t')
      {
	 nt = tlen - (cur % tlen);
	 cur += nt - 1;
	 nl = e - beg + nt;
	 rl += nl;
	 ret = realloc(ret, rl + 1);
	 memcpy(ret + pos, beg, e - beg);
	 pos += e - beg;
	 memset(ret + pos, ch, nt);
	 pos += nt;
	 beg = e + 1;
      }
   }
   nl = e - beg;
   rl += nl;
   ret = realloc(ret, rl + 1);
   memcpy(ret + pos, beg, nl);
   ret[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, rl);
   return 0;
}
