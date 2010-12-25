int
clip_CHARMIRR(ClipMachine * ClipMachineMemory)
{
   int l, l1, i;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARMIRR");
   }
   l = l1;
   ret = malloc(l + 1);
   for (l = 0, i = l1; i >= 0; i--, l++)
      ret[l] = str[i];
   ret[l] = 0;
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
