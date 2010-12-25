int
clip_ADDASCII(ClipMachine * ClipMachineMemory)
{
   int l;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   int asc = _clip_parni(ClipMachineMemory, 2);

   int t2 = _clip_parinfo(ClipMachineMemory, 2);

   int pos = _clip_parni(ClipMachineMemory, 3);

   int rset = (*(unsigned char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL || t2 != NUMERIC_type_of_ClipVarType)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ADDASCII");
   }
   if (_clip_parinfo(ClipMachineMemory, 0) < 3)
      pos = l;
   if (pos <= 0)
      pos = 1;
   if (pos > l)
      pos = l;
   pos--;
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   ret[l] = 0;
   ret[pos] = str[pos] + asc;
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
