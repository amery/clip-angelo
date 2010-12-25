int
clip_POSREPL(ClipMachine * ClipMachineMemory)
{
   int l1, l2, rl, i;

   unsigned char *ret;

   unsigned char *s1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *s2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int pos = _clip_parni(ClipMachineMemory, 3);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (s1 == NULL || s2 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "POSREPL");
   }
   if (pos <= 0)
      pos = l1 - l2 + 1;
   if (pos > l1)
   {
      _clip_retcn(ClipMachineMemory, (char *) s1, l1);
      return 0;
   }
   pos--;
   rl = (pos + l2) > l1 ? pos + l2 : l1;
   ret = malloc(rl + 1);
   memcpy(ret, s1, pos);
   memcpy(ret + pos, s2, l2);
   for (i = pos + l2; i < l1; i++)
      ret[i] = s1[i];
   ret[rl] = 0;
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, rl);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, rl);
   return 0;
}
