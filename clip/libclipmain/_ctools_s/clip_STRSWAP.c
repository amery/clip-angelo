int
clip_STRSWAP(ClipMachine * ClipMachineMemory)
{
   int l1, l2, l, i;

   unsigned char ch;

   unsigned char *s1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *s2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   if (s1 == NULL || s2 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STRSWAP");
   }
   l = l1 < l2 ? l1 : l2;
   for (i = 0; i < l; i++)
   {
      ch = s1[i];
      s1[i] = s2[i];
      s2[i] = ch;
   }
   _clip_retc(ClipMachineMemory, "");
   return 0;
}
