int
clip_CHARMIX(ClipMachine * ClipMachineMemory)
{
   int l1, l2, i;

   unsigned char *ret;

   unsigned char *str1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   if (str1 == NULL || str2 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARMIX");
   }
   if (l2 == 0)
   {
      _clip_retcn(ClipMachineMemory, (char *) str1, l1);
      return 0;
   }

   ret = malloc(l1 * 2 + 1);
   for (i = 0; i < l1; i++)
   {
      ret[i * 2] = str1[i % l1];
      ret[i * 2 + 1] = str2[i % l2];
   }
   ret[l1 * 2] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l1 * 2);
   return 0;
}
