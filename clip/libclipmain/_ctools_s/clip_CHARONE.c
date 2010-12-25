int
clip_CHARONE(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2, i;

   unsigned char *ret, *str, *s, *e, *end, *f, p;

   unsigned char *str1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   if (str1 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARONE");
   }
   s = calloc(256, 1);
   if (str2 != NULL)
   {
      l = l2;
      str = str2;
      for (e = str1, end = str1 + l1; e < end; e++)
	 s[(int) (*e)] = 1;
   }
   else
   {
      l = l1;
      str = str1;
      memset(s, 1, 256);
   }
   ret = malloc(l + 1);
   for (p = 0, e = str, end = str + l, i = 0; e < end; e++)
   {
      f = s + (*e);
      if ((*f) == 0 || ((*f) == 1 && p != *e))
      {
	 ret[i] = *e;
	 i++;
      }
      p = *e;
   }
   free(s);
   ret[i] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, i);
   return 0;
}
