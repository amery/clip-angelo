static double
_cdx_double(unsigned long long l)
{
   int i;

   double d;

   unsigned long long ll = l;

   unsigned char *cl = (unsigned char *) &l;

   unsigned char *cd = (unsigned char *) &d;

   unsigned int *il = (unsigned int *) &l;

   cl[0] = cl[0] ^ 0x80;
   for (i = 0; i < 8; i++)
      cd[i] = cl[7 - i];
   if (ll != _cdx_longlong(d))
   {
      cl[0] = cl[0] ^ 0x80;
      il[0] = ~(il[0]);
      il[1] = ~(il[1]);
      for (i = 0; i < 8; i++)
	 cd[i] = cl[7 - i];
   }
   return d;
}
