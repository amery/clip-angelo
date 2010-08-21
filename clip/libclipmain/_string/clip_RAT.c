int
clip_RAT(ClipMachine * ClipMachineMemory)
{
   int sl, l, ll, i = 1;

   const char *ss;

   char *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char *str = _clip_parcl(ClipMachineMemory, 2, &l);

   int kol = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL || s == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RAT");
   }
   if (kol <= 0)
      kol = 1;
   ss = str + l;
   while (1)
   {
      ll = ss - str;
      ss = _atr(str, s, ll, sl);
      if (i == kol && ss != NULL)
      {
	 _clip_retni(ClipMachineMemory, ss - str + 1);
	 break;
      }
      if (ss == NULL)
      {
	 _clip_retni(ClipMachineMemory, 0);
	 break;
      }
      i++;
      ss--;

   }
   return 0;
}
