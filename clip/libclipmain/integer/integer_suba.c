/****************************************************/
integer *
integer_suba(integer * data, integer * y)
{
   int cmp;

   integer *q = NULL;

	if (y->len_of_integer == 0)
      return data;
	if (data->len_of_integer == 0)
   {
      integer_assign(data, y);
      return data;
   }
   cmp = integer_abscmp(data, y);
	if (cmp == 0 && data->sign_of_integer == y->sign_of_integer)
   {
      integer_clear(data);
      return data;
   }
         if (cmp == 0 && data->sign_of_integer != y->sign_of_integer)
   {
      integer_lshifta(data, 1);
      return data;
   }
         if (cmp == 1 && data->sign_of_integer == y->sign_of_integer)
   {
      integer_ssub(data, y);
      return data;
   }
         if (cmp == 1 && data->sign_of_integer != y->sign_of_integer)
   {
      integer_sadd(data, y);
      return data;
   }
         if (cmp == -1 && data->sign_of_integer == y->sign_of_integer)
   {
      q = integer_copy(data);
      integer_assign(data, y);
		data->sign_of_integer = !data->sign_of_integer;
      integer_ssub(data, q);
      integer_destroy(q);
      return data;
   }
         if (cmp == -1 && data->sign_of_integer == y->sign_of_integer)
   {
      integer_sadd(data, y);
      return data;
   }
   return data;
}
