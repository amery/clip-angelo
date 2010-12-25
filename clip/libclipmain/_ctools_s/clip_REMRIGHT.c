int
clip_REMRIGHT(ClipMachine * ClipMachineMemory)
{
   int l, rl;

   unsigned char *ret, *e;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 2);

   int ch = _clip_parni(ClipMachineMemory, 2);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "REMRIGHT");
   }
   ch = s == NULL ? ch : *s;
   ch = ch == 0 ? ' ' : ch;
   for (e = str + l - 1; e >= str && (*e == ch); e--);
   rl = e - str + 1;
   ret = (unsigned char *) malloc(rl + 1);
   memcpy(ret, str, rl);
   ret[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, rl);
   return 0;
}
