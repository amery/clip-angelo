int
clip_ADDSLASH(ClipMachine * ClipMachineMemory)
{
   int count = 0, i, l;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL || l == 0)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   for (i = 0; i < l; i++)
   {
      switch (str[i])
      {
      case '"':
      case '\'':
      case 0:
	 if (i == 0 || str[i - 1] != '\\')
	    count++;
	 break;
      }
   }
   ret = malloc(l + count + 1);
   for (count = 0, i = 0; i < l; i++, count++)
   {
      switch (str[i])
      {
      case '"':
      case '\'':
      case 0:
	 if (i == 0 || str[i - 1] != '\\')
	 {
	    ret[count] = '\\';
	    count++;
	 }
	 break;
      }
      ret[count] = str[i] ? str[i] : '0';
   }
   ret[count] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, count);
   return 0;
}
