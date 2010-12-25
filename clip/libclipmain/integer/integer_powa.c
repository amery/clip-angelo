/*********************************************/
integer *
integer_powa(integer * data, long y)
{
   integer *r = NULL, *a = NULL;

   int rl;

	int sgn = (data->sign_of_integer && (y % 2 == 1));

	data->sign_of_integer = 0;
   if (y < 0)
   {
      integer_clear(data);
      return data;
   }
   if (y == 0)
   {
      integer_resize(data, 1);
		data->vec_of_integer[0] = 1;
      return data;
   }

   for (rl = data->len_of_integer - 1; rl >= 0 && data->vec_of_integer[rl] == 0; rl--);
   if (rl < 0 || y == 1)
      return data;

	data->sign_of_integer = sgn;
	if (rl == 0 && data->vec_of_integer[0] == 1)
      return data;

   r = integer_long_init(1L);
   a = integer_long_init(0L);
   while (1)
   {
      if (y & 1)
	 integer_mula(r, data);
      if ((y >>= 1) == 0)
	 break;
      else
      {
	 integer_destroy(a);
	 a = integer_copy(data);
	 integer_mula(data, a);
      }
   }
   //this=r;
   integer_assign(data, r);
	data->sign_of_integer = sgn;
   integer_destroy(r);
   integer_destroy(a);
   return data;
}
