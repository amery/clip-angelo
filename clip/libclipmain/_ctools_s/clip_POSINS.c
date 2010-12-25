int
clip_POSINS(ClipMachine * ClipMachineMemory)
{
   int l1, l2, rl;

   unsigned char *ret;

   unsigned char *s1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *s2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int pos = _clip_parni(ClipMachineMemory, 3);

   if (s1 == NULL || s2 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "POSINS");
   }
   if (pos <= 0)
      pos = l1;
   if (pos > l1)
   {
      _clip_retcn(ClipMachineMemory, (char *) s1, l1);
      return 0;
   }
   pos--;
   rl = l1 + l2;
   ret = malloc(rl + 1);
   memcpy(ret, s1, pos);
   memcpy(ret + pos, s2, l2);
   memcpy(ret + pos + l2, s1 + pos, l1 - pos + 1);
   ret[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, rl);
   return 0;
}
