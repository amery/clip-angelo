/**********************************************/
integer *
integer_long2_init(long i, long j)
{
   integer *ret = (integer *) calloc(sizeof(integer), 1);

   if (j < 0)
   {
		ret->sign_of_integer = 1;
      j = -j;
   }
#ifdef INTEGER_SHORTB
   ret->len_of_integer = 8;
#else
   ret->len_of_integer = 4;
#endif
   ret->vec_of_integer = (INTEGER_VECTOR) calloc(sizeof(INTEGER_HALF_TYPE) * ret->len_of_integer, 1);
#ifdef INTEGER_SHORTB
   ret->vec_of_integer[0] = EXTRACT(i);
   i >>= INTEGER_HALF;
	ret->vec_of_integer[1] = EXTRACT(i);
   i >>= INTEGER_HALF;
	ret->vec_of_integer[2] = EXTRACT(i);
   i >>= INTEGER_HALF;
	ret->vec_of_integer[3] = EXTRACT(i);
	ret->vec_of_integer[4] = EXTRACT(j);
   j >>= INTEGER_HALF;
	ret->vec_of_integer[5] = EXTRACT(j);
   j >>= INTEGER_HALF;
	ret->vec_of_integer[6] = EXTRACT(j);
   j >>= INTEGER_HALF;
	ret->vec_of_integer[7] = EXTRACT(j);
#else
   ret->vec_of_integer[0] = EXTRACT(i);
   i >>= INTEGER_HALF;
	ret->vec_of_integer[1] = EXTRACT(i);
	ret->vec_of_integer[2] = EXTRACT(j);
   j >>= INTEGER_HALF;
	ret->vec_of_integer[3] = EXTRACT(j);
#endif
   return ret;
}
