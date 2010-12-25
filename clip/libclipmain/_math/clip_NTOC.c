int
clip_NTOC(ClipMachine * ClipMachineMemory)
{
   int i = 256, b, j, l;

   char *ret, *buf, fill;

   static char base_sym[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

   unsigned long data = (unsigned long) _clip_parnl(ClipMachineMemory, 1);

   char *sdata = _clip_parc(ClipMachineMemory, 1);

   int base = _clip_parni(ClipMachineMemory, 2);

   int len = _clip_parni(ClipMachineMemory, 3);

   char *str = _clip_parc(ClipMachineMemory, 4);

   int neg = 0;

   if (base <= 0 || base > 36)
      base = 10;
   if (sdata != NULL)
      data = strtol(sdata, NULL, 16);
   if (data < 0)
   {
      neg = 1;
      data = 0 - data;
   }
   fill = str == NULL ? ' ' : (*str);
   buf = malloc(i + 1);
   buf[i] = 0;
   memset(buf, fill, i);
   for (b = 1; i >= 0; i--, b++)
   {
      j = data % base;
      buf[i] = base_sym[j];
      data = data / base;
      if (data == 0)
	 break;
   }
   if (neg)
      i--;
   l = 256 - i + 1;
   if (len <= 0)
      len = l;
   ret = calloc(len + 1, 1);
   if (len < l)
   {
      memset(ret, '*', len);
      free(buf);
      _clip_retcn_m(ClipMachineMemory, ret, len);
      return 0;
   }
   memset(ret, fill, len);
   memcpy(ret + len - l, buf + i, l);
   if (neg)
   {
      if (fill == ' ')
	 ret[len - l] = '-';
      else
	 *ret = '-';

   }
   free(buf);
   _clip_retcn_m(ClipMachineMemory, ret, len);
   return 0;
}
