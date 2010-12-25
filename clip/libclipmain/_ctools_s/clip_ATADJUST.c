int
clip_ATADJUST(ClipMachine * ClipMachineMemory)
{
   int l, le, l1, l2;

   unsigned char fillchr = ' ', *beg, *ret;

   unsigned char *sstr = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int newpos = _clip_parni(ClipMachineMemory, 3);

   int count = _clip_parni(ClipMachineMemory, 4);

   int ignore = _clip_parni(ClipMachineMemory, 5);

   int t6 = _clip_parinfo(ClipMachineMemory, 6);

   if (sstr == NULL || str == NULL || _clip_parinfo(ClipMachineMemory, 0) < 3)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ATADJUST");
   }
   if (t6 == NUMERIC_type_of_ClipVarType)
      fillchr = _clip_parni(ClipMachineMemory, 6);
   if (t6 == CHARACTER_type_of_ClipVarType)
      fillchr = *(_clip_parc(ClipMachineMemory, 6));

   beg = _clip_atmupa(ClipMachineMemory, sstr, l1, str, l2, ignore, count, 1);
   if (beg < str + l2)
   {
      le = beg - str;
      if (le >= newpos)
	 le = newpos - 1;
      l = newpos + str + l2 - beg;
      ret = malloc(l + 1);
      memcpy(ret, str, le);
      for (; le < newpos; le++)
	 ret[le] = fillchr;
      memcpy(ret + le, beg, l - le);
      ret[l] = 0;
      _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   }
   else
      _clip_retcn(ClipMachineMemory, (char *) str, l2);
   return 0;
}
