static unsigned long long
_cdx_longlong(double d)
{
   int i;

   unsigned long long res;

   unsigned char *cd = (unsigned char *) &res;

   unsigned int *id = (unsigned int *) &res;

   for (i = 0; i < 8; i++)
      ((unsigned char *) &res)[i] = ((unsigned char *) &d)[7 - i];
   if (d < 0)
   {
      id[0] = ~(id[0]);
      id[1] = ~(id[1]);
   }
   else
   {
      cd[0] = cd[0] ^ 0x80;
   }
   return res;
}
