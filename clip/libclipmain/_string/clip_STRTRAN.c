int
clip_STRTRAN(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2, rl = 0, n;

   char *ret, *e, *m, *end, *end1, *end2, *e1, *e2;

   char *str = _clip_parcl(ClipMachineMemory, 1, &l);

   char *s1 = _clip_parcl(ClipMachineMemory, 2, &l1);

   char *s2 = _clip_parcl(ClipMachineMemory, 3, &l2);

   int from = _clip_parni(ClipMachineMemory, 4);

   int kol = _clip_parni(ClipMachineMemory, 5);

   if (str == NULL || s1 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STRTRAN");
   }
   if (s2 == NULL)
   {
      s2 = "";
      l2 = 0;
   }
   if (from <= 0)
      from = 1;
   if (kol <= 0)
      kol = 0;
   from--;
   end = str + l;
   end1 = s1 + l1;
   end2 = s2 + l2;
   if (kol == 0)
   {
      for (e = str + from; e < end; e++)
      {
	 for (e1 = s1, e2 = e; e1 < end1 && e2 < end && (*e1) == (*e2); e1++, e2++);
	 if (e1 == end1)
	 {
	    kol++;
	    e += l1 - 1;
	 }
	 if (e >= end)
	    break;
      }
   }
   if (kol == 0)
   {
      _clip_retcn(ClipMachineMemory, str, l);
      return 0;
   }
   rl = l + (l2 - l1) * kol + 1;
   ret = malloc(rl + 1);
   memcpy(ret, str, from);
   for (e = str + from, m = e, n = 0; e < end && n < kol; e++)
   {
      for (e1 = s1, e2 = e; e1 < end1 && e2 < end && (*e1) == (*e2); e1++, e2++);
      if (e1 != end1)
	 continue;
      memcpy(ret + from, m, e - m);
      from += e - m;
      memcpy(ret + from, s2, l2);
      from += l2;
      if (e1 == end1)
      {
	 e += l1 - 1;
	 m = e + 1;
	 n++;
      }
   }
   memcpy(ret + from, m, str + l - m);
   from += str + l - m;
   ret[from] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, from);
   return 0;
}
