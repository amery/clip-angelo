/****************************************************/
char *
rational_toString(rational * data, int base, int dec, int group)
{
   int i, ptr = 0;

   char *ret, *buf;

   integer *koef = integer_long_init(base);

   if (dec < 0)
      dec = 0;

   integer_powa(koef, dec + 1);
   integer_mula(koef, data->num);
   integer_diva(koef, data->den);

   if (integer_empty(data->num) || integer_empty(koef))
   {
      ret = calloc(dec + 3, 1);
      ret[0] = '0';
      ret[1] = '.';
      for (i = 0; i < dec; i++)
	 ret[i + 2] = '0';
      integer_destroy(koef);
      return ret;
   }

   buf = integer_toString(koef, base);
   i = strlen(buf);
   if (i > 1)
   {
      if (buf[i - 1] >= '5')
	 buf[i - 2]++;
   }
   if (dec != 0)
   {
      dec++;
      if (dec == i)
	 ptr++;
      ret = calloc(i + 2 + ptr, 1);
      if (dec == i)
	 ret[0] = '0';
      memcpy(ret + ptr, buf, i - dec);
      ret[i - dec + ptr] = '.';
      memcpy(ret + ptr + i - dec + 1, buf + i - dec, dec - 1);
   }
   else
   {
      dec++;
      if (dec == i)
      {
	 ret = calloc(2, 1);
	 ret[0] = '0';
      }
      else
      {
	 ret = calloc(i, 1);
	 memcpy(ret, buf, i - 1);
      }
   }
   free(buf);

   integer_destroy(koef);
   return ret;
}
