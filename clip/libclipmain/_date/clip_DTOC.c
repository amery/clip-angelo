int
clip_DTOC(ClipMachine * ClipMachineMemory)
{
   long d = _clip_pardj(ClipMachineMemory, 1);

   int flag1 = 0, flag2 = 0, flag3 = 0;

   int fox = 0, i, len;

   int pcount = _clip_parinfo(ClipMachineMemory, 0);

   char *format = _clip_parcl(ClipMachineMemory, 2, &len);

   char *dtoc = NULL, ch;

   if (pcount == 1)
      format = ClipMachineMemory->date_format;
   else
   {
      if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      {
	 fox = 1;
	 for (i = 0; i < len; i++)
	 {
	    ch = format[i];
	    switch (ch)
	    {
	    case 'Y':
	    case 'y':
	       flag1++;
	       break;
	    case 'M':
	    case 'm':
	       flag2++;
	       break;
	    case 'D':
	    case 'd':
	       flag3++;
	       break;
	    }
	 }
	 if (flag1 && flag2 && flag3)
	    fox = 0;
      }
      else
	 fox = 1;

   }

   if (fox)
      dtoc = _clip_date_to_str(d, "YYYYMMDD");
   else
      dtoc = _clip_date_to_str(d, format);

   _clip_retcn_m(ClipMachineMemory, dtoc, strlen(dtoc));
   return 0;
}
