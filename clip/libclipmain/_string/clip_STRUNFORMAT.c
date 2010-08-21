int
clip_STRUNFORMAT(ClipMachine * ClipMachineMemory)
{
   int i, len, nw;

   char *str1;

   char *str2 = _clip_parcl(ClipMachineMemory, 1, &len);

   if (str2 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STRUNFORMAT");
   }
   for (nw = 0; str2[nw] == ' '; nw++);
   for (i = 0; i < len;)
   {
      nw++;
      if (str2[i] == ' ')
	 while (str2[i] == ' ')
	    i++;
      else
	 i++;
   }
   str1 = (char *) malloc(nw + 1);

   for (i = 0, nw = 0; str2[i] == ' '; i++, nw++)
      str1[nw] = str2[i];
   for (; i < len;)
   {
      str1[nw] = str2[i];
      nw++;
      if (str2[i] == ' ')
	 while (str2[i] == ' ')
	    i++;
      else
	 i++;
   }
   str1[nw] = 0;
   _clip_retcn_m(ClipMachineMemory, str1, nw);
   return 0;
}
