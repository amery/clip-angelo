int
clip_EXPAND(ClipMachine * ClipMachineMemory)
{
   int l = 0, l1, l2, i;

   unsigned char *e, *ret, *cur, *end, ch = ' ';

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   int kol = _clip_parni(ClipMachineMemory, 2);

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   if (_clip_parinfo(ClipMachineMemory, 2) != CHARACTER_type_of_ClipVarType)
      s = (unsigned char *) _clip_parcl(ClipMachineMemory, 3, &l2);
   if (s != NULL)
      ch = *s;
   if (kol <= 0)
      kol = 1;

   if (str == NULL || l1 == 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   l = (l1 - 1) * (kol + 1) + 1;
   ret = malloc(l + 1);
   ret[l] = 0;
   for (cur = ret, e = str, end = str + l1 - 1; e < end; e++)
   {
      *cur = *e;
      for (i = 0, cur++; i < kol; i++, cur++)
	 *cur = ch;
   }
   *cur = *e;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
