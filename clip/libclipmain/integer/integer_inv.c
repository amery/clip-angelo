/********************************************/
integer *
integer_inv(integer * data)
{
   integer *ret = integer_copy(data);

	ret->sign_of_integer = !ret->sign_of_integer;
   return ret;
}
