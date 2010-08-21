int
clip_CHARADD(ClipMachine * ClipMachineMemory)
{
   int l1, l2;

   unsigned char *ret, *e, *end;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int asc = _clip_parni(ClipMachineMemory, 2);

   int t2 = _clip_parinfo(ClipMachineMemory, 2);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARADD");
   }
   if (t2 == CHARACTER_type_of_ClipVarType)
      asc = *s;
   ret = malloc(l1 + 1);
   memcpy(ret, str, l1);
   ret[l1] = 0;
   for (e = ret, end = ret + l1; e < end; e++)
      *e += asc;
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l1);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l1);
   return 0;
}
