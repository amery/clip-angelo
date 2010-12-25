int
clip_STRFORMAT(ClipMachine * ClipMachineMemory)
{
   int i, len, nw, lw, n, c, c1;

   char *str1;

   char *str2 = _clip_parcl(ClipMachineMemory, 1, &len);

   int lend = _clip_parni(ClipMachineMemory, 2);

   if (str2 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "STRFORMAT");
   }

   str1 = (char *) malloc(lend + 1);

   for (len--; str2[len] == ' '; len--);
   str2[len + 1] = 0;
   if (len > lend)
      str2[lend + 1] = 0;

   for (i = 0; str2[i] == ' '; i++);
   for (; str2[i] != ' ' && str2[i] != 0; i++);
   n = i;
   for (nw = 0, lw = 0; str2[i] != 0; nw++)
   {
      for (; str2[i] == ' '; i++);
      for (; str2[i] != ' ' && str2[i] != 0; i++, lw++);
   }
   if (nw != 0)
   {
      c = (lend - n - lw) / nw;
      c1 = lend - n - lw - c * nw;
   }
   else
   {
      c = lend - n;
      c1 = 0;
      nw = 1;
   }
   for (i = 0; i < n; i++)
      str1[i] = str2[i];
   for (; nw > 0; nw--)
   {
      for (lw = 0; lw < c; lw++, n++)
	 str1[n] = ' ';
      if (c1 > 0)
      {
	 str1[n] = ' ';
	 n++;
	 c1--;
      }
      for (; str2[i] == ' '; i++);
      for (; str2[i] != ' ' && str2[i] != 0; i++, n++)
	 str1[n] = str2[i];

   }
   str1[n] = 0;
   _clip_retcn_m(ClipMachineMemory, str1, n);
   return 0;
}
