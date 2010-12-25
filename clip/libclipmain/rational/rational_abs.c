/*********************************************/
rational *
rational_abs(rational * data)
{
   rational *ret = rational_copy(data);

	if (ret->num->sign_of_integer)
      integer_inverse(ret->num);
   return ret;
}
