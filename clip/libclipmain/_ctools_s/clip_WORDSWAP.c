int
clip_WORDSWAP(ClipMachine * ClipMachineMemory)
{
   int l, i;

   short *ret = 0, ch;

   short *str = (short *) _clip_parcl(ClipMachineMemory, 1, &l);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   l = l >> 1;

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "WORDSWAP");
   }
   ret = malloc((l + 1) * 2);
   for (i = 0; i < l; i += 2)
   {
      if (i < l - 1)
      {
	 ch = str[i];
	 ret[i] = str[i + 1];
	 ret[i + 1] = ch;
      }
      else
	 ret[i] = str[i];
   }
   ret[l] = 0;
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l * 2);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l * 2);
   return 0;
}
