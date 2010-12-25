int
clip_TABPACK(ClipMachine * ClipMachineMemory)
{
   int l, rl, cur = 1, pos = 0, kol = 0;

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
   rl = l;
   ret = malloc(rl + 1);
   for (e = str, beg = str, end = str + l + 1; e < end; e++, cur++)
   {
      cch = *e;
      if (cch == '\n' || cch == '\t')
      {
	 cur = -1;
	 kol = 0;
	 continue;
      }
      if (cch == ch)
	 kol++;
      else
      {
	 kol = 0;
	 continue;
      }
      //if (((cur + tlen) % tlen) != 0 && kol < tlen)
      if (((cur + tlen) % tlen) != 0 /*&& kol < 2 */ )
	 continue;
      if (kol < 2)
	 continue;
      if (cch == ch)
	 kol--;
      else if (cch != 0)
      {
	 kol = 0;
	 continue;
      }
      memcpy(ret + pos, beg, e - beg - kol);
      pos += e - beg - kol;
      ret[pos] = '\t';
      pos++;
      beg = e + (cch != 0);
      kol = 0;

   }
   memcpy(ret + pos, beg, e - beg);
   pos += e - beg - 1;
   ret[pos] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, pos);
   return 0;
}
