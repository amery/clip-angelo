int
clip_CHARLIST(ClipMachine * ClipMachineMemory)
{
   int l, l1, i;

   unsigned char *ret, *buf, *e, *end, *s;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHARLIST");
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
   ret = malloc(l + 1);
   for (i = 0, e = buf, end = buf + 256; e < end; e++)
   {
      if (*e == 0)
	 continue;
      ret[i] = e - buf;
      i++;
   }
   ret[l] = 0;
   free(buf);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
