int
clip_POSCHAR(ClipMachine * ClipMachineMemory)
{
   int l;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 2);

   int ch = _clip_parni(ClipMachineMemory, 2);

   int pos = _clip_parni(ClipMachineMemory, 3);

   int rset = (*(char *) (_clip_fetch_item(ClipMachineMemory, HASH_csetref)) == 't');

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "POSCHAR");
   }
   if (pos <= 0)
      pos = l;
   pos--;
   if (ch <= 0)
      ch = *s;
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   ret[pos] = ch;
   if (rset && _clip_par_isref(ClipMachineMemory, 1))
      _clip_par_assign_str(ClipMachineMemory, 1, (char *) ret, l);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
