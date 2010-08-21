int
clip_CSCOUNT(ClipMachine * ClipMachineMemory)
{
   int i, j, l1, l2;

   unsigned char ch;

   unsigned char *str1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   if (str1 == NULL || str2 == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }

   ch = *str1;
   for (i = 0, j = 0; i < l2; i++)
      if (ch == str2[i])
	 j++;
   _clip_retni(ClipMachineMemory, j);
   return 0;
}
