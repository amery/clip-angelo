int
clip_POSDEL(ClipMachine * ClipMachineMemory)
{
   int l, rl, i;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   int pos = _clip_parni(ClipMachineMemory, 2);

   int num = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "POSDEL");
   }
   if (num <= 0)
      num = 1;
   if (pos <= 0)
      pos = l - num + 1;
   if (pos > l)
   {
      pos = l;
      num = 0;
   }
   pos--;
   rl = l - num;
   ret = malloc(rl + 1);
   memcpy(ret, str, pos);
   for (i = pos + num; i < l; i++, pos++)
      ret[pos] = str[i];
   ret[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, rl);
   return 0;
}
