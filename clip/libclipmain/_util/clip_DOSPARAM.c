int
clip_DOSPARAM(ClipMachine * ClipMachineMemory)
{
   char *s;

   int i, l;

   for (i = 1, l = 0; i < _clip_raw_argc; i++)
      l += strlen(_clip_raw_argv[i]) + 1;

   s = (char *) malloc(l + 1);

   for (i = 1, l = 0; i < _clip_raw_argc; i++)
   {
      char *p = _clip_raw_argv[i];

      int l1 = strlen(p);

      memcpy(s + l, p, l1);
      s[l + l1] = ' ';
      l += l1 + 1;
   }

   if (l)
      l--;
   s[l] = 0;

   _clip_retcn_m(ClipMachineMemory, s, l);

   return 0;
}
