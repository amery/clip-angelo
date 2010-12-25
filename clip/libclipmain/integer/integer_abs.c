/*********************************************/
integer *
integer_abs(integer * data)
{
   integer *ret = integer_copy(data);

	ret->sign_of_integer = 0;
   return ret;
}
