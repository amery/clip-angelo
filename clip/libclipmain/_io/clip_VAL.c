int
clip_VAL(ClipMachine * ClipMachineMemory)
{
   int l;

   char *s = _clip_parcl(ClipMachineMemory, 1, &l);

   int plen = _clip_parni(ClipMachineMemory, 2);

   int pdec = _clip_parni(ClipMachineMemory, 3);

   int i;

   if (!s)
   {
      _clip_retnd(ClipMachineMemory, 0);
      return 0;
   }
   if (ClipMachineMemory->flags & RATIONAL_FLAG)
   {
      for (i = l; i >= 0 && s[i] != '/' && s[i] != '.'; i--);
      if (i < 0)
	 i = 0;
      else
	 i = l - i - 1;
      _clip_retnr(ClipMachineMemory, rational_fromString(s), l, i);
   }
   else
   {
      char *sp;

      int dec = 0, len = 0;

      double d;

      d = _clip_strtod(s, &dec);
      if (dec == 0)
      {
	 for (sp = s; *sp && (*sp == ' ' || *sp == '\t'); sp++)
	    ;
      }
      else
	 sp = s + dec;
      for (; *sp && (isdigit(*sp) || *sp == '.' || *sp == ',' || *sp == '-' || *sp == '+' || *sp == ' '); ++sp)
	 ;
      len = sp - s;
      if (dec)
	 dec = len - dec - 1;

      if (ClipMachineMemory->flags & FIXED_FLAG)
	 dec = ClipMachineMemory->decimals;
      if (dec < 0 || dec == len)
	 dec = 0;
      if (plen != 0)
	 len = plen;
      if (len < dec + 2)
	 len = dec + 2;
      if (_clip_parinfo(ClipMachineMemory, 0) == 3)
	 dec = pdec;
      _clip_retndp(ClipMachineMemory, d, len, dec);
   }

   return 0;
}
