/*********************************************/
integer *
integer_sqrt(integer * data)
{
   integer *ret = integer_copy(data);

   integer *r = integer_copy(data);

   integer *q = NULL;

	if (data->sign_of_integer || integer_empty(data))
   {
      integer_destroy(r);
      return ret;
   }
   integer_rshifta(ret, integer_lg(ret) / 2);
   integer_sdivide(r, ret, &q);
   while (integer_abscmp(r, ret) < 0)
   {
      integer_adda(ret, r);
      integer_rshifta(ret, 1);
      integer_destroy(r);
      r = integer_copy(data);
      integer_sdivide(r, ret, &q);
   }
   integer_destroy(r);
   integer_destroy(q);
   return ret;
}
