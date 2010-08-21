int
clip_REPLALL(ClipMachine * ClipMachineMemory)
{
   int l;

   unsigned char *ret, *e, *end;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   unsigned char *s1 = (unsigned char *) _clip_parc(ClipMachineMemory, 2);

   int ch1 = _clip_parni(ClipMachineMemory, 2);

   unsigned char *s2 = (unsigned char *) _clip_parc(ClipMachineMemory, 3);

   int ch2 = _clip_parni(ClipMachineMemory, 3);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL || s1 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "REPLALL");
   }
   ch1 = s1 == NULL ? ch1 : *s1;
   ch1 = ch1 == 0 ? ' ' : ch1;
   ch2 = s2 == NULL ? ch2 : *s2;
   ch2 = ch2 == 0 ? ' ' : ch2;
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   for (e = ret, end = ret + l; e < end && (*e == ch2); e++)
      *e = ch1;
   for (e = ret + l - 1; e >= ret && (*e == ch2); e--)
      *e = ch1;
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
