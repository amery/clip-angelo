int
clip_CHARONLY(ClipMachine * ClipMachineMemory)
{
   int l, l1, l2, i;

   unsigned char *ret, *e, *end, *s;

   unsigned char *str1 = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *str2 = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   if (str1 == NULL || str2 == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARONLY");
   }
   l = l2;
   ret = malloc(l + 1);
   s = calloc(256, 1);
   for (e = str1, end = str1 + l1; e < end; e++)
      s[(int) (*e)] = 1;
   for (i = 0, e = str2, end = str2 + l2; e < end; e++)
   {
      if (s[(int) (*e)] == 1)
      {
	 ret[i] = *e;
	 i++;
      }
   }
   free(s);
   ret[i] = 0;
   _clip_retcn_m(ClipMachineMemory, (char *) ret, i);
   return 0;
}
