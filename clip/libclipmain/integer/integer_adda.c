integer *
integer_adda(integer * data, integer * z)
{
   integer *q = NULL;

   int cmp;

   if (integer_empty(z))
      return data;
	if (data->len_of_integer == 0)
   {
      integer_assign(data, z);
      return data;
   }
   cmp = integer_abscmp(data, z);
	if (cmp == 0 && data->sign_of_integer != z->sign_of_integer)
   {
      integer_clear(data);
      return data;
   }
         if (cmp == 0 && data->sign_of_integer == z->sign_of_integer)
   {
      integer_lshifta(data, 1);
      return data;
   }
         if (cmp == 1 && data->sign_of_integer != z->sign_of_integer)
   {
      integer_ssub(data, z);
      return data;
   }
         if (cmp == 1 && data->sign_of_integer == z->sign_of_integer)
   {
      integer_sadd(data, z);
      return data;
   }
         if (cmp == -1 && data->sign_of_integer != z->sign_of_integer)
   {
      q = integer_copy(data);
      integer_assign(data, z);
      integer_ssub(data, q);
      integer_destroy(q);
      return data;
   }
         if (cmp == -1 && data->sign_of_integer == z->sign_of_integer)
   {
      integer_sadd(data, z);
      return data;
   }
   return data;
}
