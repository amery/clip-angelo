static int
_cdx_dupbytes(void *key1, void *key2, int len)
{
   int i;

   int *k1, *k2;

   int l = len / sizeof(int);

   int o = len % sizeof(int);

   for (i = 0, k1 = key1, k2 = key2; i < l && *k1 == *k2 && *k1; i++, k1++, k2++)
      ;
   o += (l - i) * sizeof(int);
   if (o || i < l)
   {
      i = i << 2;
      if (o == 1)
      {
	 i += ((*(char *) k1 == *(char *) k2) && (*(char *) k1));
	 return i;
      }
      if ((*(short *) k1 == *(short *) k2) && (*(short *) k1))
      {
	 i += 2;
	 if (o > 2)
	    i += (*(((char *) k1) + 2) == *(((char *) k2) + 2) && (*(char *) k1));
      }
      else
      {
	 i += ((*(char *) k1 == *(char *) k2) && (*(char *) k1));
      }
      return i;
   }
   return len;
}
