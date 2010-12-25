int
clip_FLOATVAL(ClipMachine * ClipMachineMemory)
{
   char *s = _clip_parc(ClipMachineMemory, 1);

   int len = _clip_parni(ClipMachineMemory, 2);

   int dec = _clip_parni(ClipMachineMemory, 3);

   if (!s)
   {
      _clip_retnd(ClipMachineMemory, 0);
      return 0;
   }
   if (strchr(s, 'e') || strchr(s, 'E'))
   {
      struct lconv *lc = localeconv();

      char c, *r;

      if (*lc->decimal_point == '.')
	 c = ',';
      else
	 c = '.';
      r = s;
      while ((r = strchr(r, c)))
	 *r = *lc->decimal_point;
      _clip_retndp(ClipMachineMemory, atof(s), len, dec);
      return 0;
   }
   return clip_VAL(ClipMachineMemory);
}
