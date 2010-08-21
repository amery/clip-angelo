int
clip_REMALL(ClipMachine * ClipMachineMemory)
{
   int l, rl;

   unsigned char *ret, *e, *end;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 2);

   int ch = _clip_parni(ClipMachineMemory, 2);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "REMALL");
   }
   ch = s == NULL ? ch : *s;
   ch = ch == 0 ? ' ' : ch;
   ret = malloc(l + 1);
   for (e = str, end = str + l; e < end && (*e == ch); e++);
   rl = l - (e - str);
   memcpy(ret, e, rl);
   for (e = ret + rl - 1; e >= ret && (*e == ch); e--, rl--)
      ret[rl] = 0;
   ret = realloc(ret, rl + 1);
   ret[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, rl);
   return 0;
}
