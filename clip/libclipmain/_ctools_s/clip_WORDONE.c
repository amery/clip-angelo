int
clip_WORDONE(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2, i;

   short *ret, *str, *e, *end, word = 0, cword = 0;

   short *str1 = (short *) _clip_parcl(ClipMachineMemory, 1, &l1);

   short *str2 = (short *) _clip_parcl(ClipMachineMemory, 2, &l2);

   l1 = l1 >> 1;
   l2 = l2 >> 1;

   if (str1 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "WORDONE");
   }
   if (str2 != NULL)
   {
      l = l2;
      str = str2;
      word = *str1;
   }
   else
   {
      l = l1;
      str = str1;
   }
   ret = malloc((l + 1) * 2);
   cword = *str;
   ret[0] = *str;
   for (e = str + 1, end = str + l, i = 1; e < end; e++)
   {
      if (*e == cword && (word == 0 || cword == word))
	 continue;
      ret[i] = *e;
      i++;
      cword = *e;
   }
   ret[i] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, i * 2);
   return 0;
}
