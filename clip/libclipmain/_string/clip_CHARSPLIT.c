int
clip_CHARSPLIT(ClipMachine * ClipMachineMemory)
{
   int i = 1, sl, l, ll, kol;

   const char *ss;

   char *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char *str = _clip_parcl(ClipMachineMemory, 2, &l);

   ClipVar *arr = RETPTR(ClipMachineMemory);

   long ln[2];

   if (str == NULL || s == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHSPLIT");
   }

   ln[0] = 0;
   _clip_array(ClipMachineMemory, arr, 1, ln);

   ss = str;
   kol = 0;
   while (1)
   {
      ll = l - (ss - str);
      ss = _atl(ss, s, ll, sl);
      if (ss != NULL)
      {
	 ClipVar *st;

	 st = NEW(ClipVar);
	 _clip_var_str(str + kol, ss - str - kol, st);
	 _clip_aadd(ClipMachineMemory, arr, st);

	 _clip_delete(ClipMachineMemory, st);

	 kol = ss - str + 1;
      }
      if (ss == NULL)
      {
	 ClipVar *st;

	 st = NEW(ClipVar);
	 _clip_var_str(str + kol, l - kol, st);
	 _clip_aadd(ClipMachineMemory, arr, st);

	 _clip_delete(ClipMachineMemory, st);
	 break;
      }
      i++;
      ss++;

   }
   return 0;
}
