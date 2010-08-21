int
clip_CHAROR(ClipMachine * ClipMachineMemory)
{
   int l1, l2;

   unsigned char *ret, *e, *end, *e2, *end2;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL || s == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHAROR");
   }
   ret = malloc(l1 + 1);
   memcpy(ret, str, l1);
   ret[l1] = 0;
   end2 = s + l2;
   for (e = ret, end = ret + l1; e < end;)
      for (e2 = s; e2 < end2 && e < end; e++, e2++)
	 *e = (*e | *e2);
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l1);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l1);
   return 0;
}
