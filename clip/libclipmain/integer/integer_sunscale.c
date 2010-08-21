/***********************************************/
integer *
integer_sunscale(integer * data, INTEGER_HALF_TYPE y, INTEGER_HALF_TYPE * ost)
{
   long rem = 0, u;

   int i;

	if (data->len_of_integer == 0 || y == 1)
   {
      (*ost) = 0;
      return data;
   }

   for (i = data->len_of_integer - 1; i >= 0; i--)
   {
		rem = UP(rem) | data->vec_of_integer[i];
      u = rem / y;
		data->vec_of_integer[i] = EXTRACT(u);
      rem -= u * y;
   }
   (*ost) = EXTRACT(rem);
   return data;
}
