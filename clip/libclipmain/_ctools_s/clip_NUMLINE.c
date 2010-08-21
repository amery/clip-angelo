int
clip_NUMLINE(ClipMachine * ClipMachineMemory)
{
   int i, l, ret = 0;

   unsigned char *e, *beg, *end;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   int len = _clip_parni(ClipMachineMemory, 2);

   if (len <= 0)
      len = 80;

   if (str == 0)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "NUMLINE");
   }
   for (e = str, beg = str, end = str + l; e <= end; e++)
   {
      for (i = 1; e < end && i < len && *e != '\n'; e++, i++)
	 if (*e == '\r')
	    i--;
      ret++;
   }
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
