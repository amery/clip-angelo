int
clip_OCCURS(ClipMachine * ClipMachineMemory)
{
   int i = 0, sl, l, ll;

   const char *ss;

   char *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   char *str = _clip_parcl(ClipMachineMemory, 2, &l);

   if (str == NULL || s == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "OCCURS");
   }
   ss = str;
   while (1)
   {
      ll = l - (ss - str);
      ss = _atl(ss, s, ll, sl);
      if (ss == NULL)
	 break;
      i++;
      ss++;

   }
   _clip_retni(ClipMachineMemory, i);
   return 0;
}
