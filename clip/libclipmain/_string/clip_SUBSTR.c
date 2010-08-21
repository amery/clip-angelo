int
clip_SUBSTR(ClipMachine * ClipMachineMemory)
{
   /* sorry test */
   int l;

   char *ret;

   char *str = _clip_parcl(ClipMachineMemory, 1, &l);

   int from = _clip_parni(ClipMachineMemory, 2);

   int kol = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "SUBSTR");
   }
   if (kol <= 0)
      kol = 0;
   if (from < 0)
   {
      from = l + from;
      if (from < 0)
	 from = 0;
   }
   else if (from)
      from--;
   if (_clip_parinfo(ClipMachineMemory, 0) < 3)
      kol = l - from;
   if ((kol + from) > l)
      kol = l - from;
   if (kol < 0)
      kol = 0;
   ret = malloc(kol + 1);
   memcpy(ret, str + from, kol);
   ret[kol] = 0;
   _clip_retcn_m(ClipMachineMemory, ret, kol);
   return 0;
}
