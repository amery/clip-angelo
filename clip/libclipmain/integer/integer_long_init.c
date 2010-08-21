/**********************************************/
integer *
integer_long_init(long i)
{
   integer *ret = (integer *) calloc(sizeof(integer), 1);

   if (i < 0)
   {
		ret->sign_of_integer = 1;
      i = -i;
   }
#ifdef INTEGER_SHORTB
   ret->len_of_integer = 4;
#else
   ret->len_of_integer = 2;
#endif
   ret->vec_of_integer = (INTEGER_VECTOR) calloc(sizeof(INTEGER_HALF_TYPE) * ret->len_of_integer, 1);
	ret->vec_of_integer[0] = EXTRACT(i);
   i >>= INTEGER_HALF;
	ret->vec_of_integer[1] = EXTRACT(i);
#ifdef INTEGER_SHORTB
   i >>= INTEGER_HALF;
	ret->vec_of_integer[2] = EXTRACT(i);
   i >>= INTEGER_HALF;
	ret->vec_of_integer[3] = EXTRACT(i);
#endif
   return ret;
}
