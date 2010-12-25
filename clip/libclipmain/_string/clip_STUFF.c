int
clip_STUFF(ClipMachine * ClipMachineMemory)
{
   int l, l1, rl;

   char *ret;

   char *str = _clip_parcl(ClipMachineMemory, 1, &l);

   int from = _clip_parni(ClipMachineMemory, 2);

   int kol = _clip_parni(ClipMachineMemory, 3);

   char *s1 = _clip_parcl(ClipMachineMemory, 4, &l1);

   if (str == NULL || s1 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STUFF");
   }
   if (from <= 0)
      from = 1;
   if (from > l)
      from = l + 1;
   if (kol <= 0)
      kol = 0;
   if ((from + kol) > l)
      kol = l - from + 1;
   from--;
   rl = l - kol + l1;
   if (rl <= 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   ret = malloc(rl + 1);
   memcpy(ret, str, from);
   memcpy(ret + from, s1, l1);
   memcpy(ret + from + l1, str + from + kol, l - from - kol);
   ret[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, rl);
   return 0;
}
