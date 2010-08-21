int
clip_RANGEREM(ClipMachine * ClipMachineMemory)
{
   int l;

   unsigned char *e, *end, *r, *ret;

   unsigned char ch, ch1, ch2;

   unsigned char *s1 = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   unsigned char *s2 = (unsigned char *) _clip_parc(ClipMachineMemory, 2);

   int t1 = _clip_parni(ClipMachineMemory, 1);

   int t2 = _clip_parni(ClipMachineMemory, 2);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l);

   if (str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RANGEREM");
   }
   ch1 = s1 == NULL ? t1 : *s1;
   ch2 = s2 == NULL ? t2 : *s2;
   ch1 = _clip_cmptbl[ch1];
   ch2 = _clip_cmptbl[ch2];
   if (ch1 > ch2)
   {
      ch = ch1;
      ch1 = ch2;
      ch1 = ch;
   }
   ret = malloc(l + 1);
   for (e = str, r = ret, end = str + l; e < end; e++)
   {
      ch = _clip_cmptbl[*e];
      if (ch >= ch1 && ch <= ch2)
	 continue;
      *r = *e;
      r++;
   }
   *r = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, r - ret);
   return 0;
}
