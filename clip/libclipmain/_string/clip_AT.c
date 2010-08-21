int
clip_AT(ClipMachine * ClipMachineMemory)
{
   int i = 1, sl, l, ll;

   const char *ss;

   char *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char *str = _clip_parcl(ClipMachineMemory, 2, &l);

   int kol = _clip_parni(ClipMachineMemory, 3);

   if (str == NULL || s == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "AT");
   }
   if (kol <= 0)
      kol = 1;
   ss = str;
   while (1)
   {
      ll = l - (ss - str);
      ss = _atl(ss, s, ll, sl);
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
      ss++;

   }
   return 0;
}
