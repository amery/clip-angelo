int
clip_CHARNOLIST(ClipMachine * ClipMachineMemory)
{
   int l, l1, i;

   unsigned char *ret, *buf, *e, *end, *s;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   if (str == NULL)
   {
      l = 256;
      ret = malloc(l + 1);
      for (i = 0; i < 256; i++)
	 ret[i] = i;
      ret[l] = 0;
      _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
      return 0;
   }
   l = 0;
   buf = calloc(256, 1);
   for (e = str, end = str + l1; e < end; e++)
   {
      s = buf + (*e);
      if (*s == 0)
	 l++;
      *s = 1;
   }
   l = 256 - l;
   ret = malloc(l + 1);
   for (i = 0, e = buf, end = buf + 256; e < end; e++)
   {
      if (*e == 1)
	 continue;
      ret[i] = e - buf;
      i++;
   }
   ret[l] = 0;
   free(buf);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
