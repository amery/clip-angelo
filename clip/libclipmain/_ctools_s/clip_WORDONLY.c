int
clip_WORDONLY(ClipMachine * ClipMachineMemory)
{
   int l1, l2, i = 0;

   short *ret, *e1, *end1, *e2, *end2;

   short *str1 = (short *) _clip_parcl(ClipMachineMemory, 1, &l1);

   short *str2 = (short *) _clip_parcl(ClipMachineMemory, 2, &l2);

   if (str1 == NULL || str2 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "WORDONLY");
   }
   l1 = l1 >> 1;
   l2 = l2 >> 1;
   ret = malloc((l2 + 1) * 2);
   for (e2 = str2, end2 = str2 + l2; e2 < end2; e2++)
   {
      for (e1 = str1, end1 = str1 + l1; e1 < end1 && *e2 != *e1; e1++);
      if (*e1 != *e2)
	 continue;
      ret[i] = *e2;
      i++;
   }
   ret[i] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, i * 2);
   return 0;
}
