/****************************************************/
double
rational_toDouble(rational * data)
{
   int i;

   double ret, d1 = 0, d2 = 0, base;

   rational_normalize(data);

	for (i = 0; i < data->num->len_of_integer; i++)
   {
      base = pow((double) 2, (double) (i * INTEGER_HALF));
		d1 += base * data->num->vec_of_integer[i];
   }
      if (data->num->sign_of_integer)
      d1 = 0 - d1;
		for (i = 0; i < data->den->len_of_integer; i++)
   {
      base = pow((double) 2, (double) (i * INTEGER_HALF));
		d2 += base * data->den->vec_of_integer[i];
   }
      if (data->den->sign_of_integer)
      d2 = 0 - d2;
   ret = d1 / d2;
   return ret;
}
