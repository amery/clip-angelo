/**********************************************/
integer *
integer_copy(integer * data)
{
   integer *ret = (integer *) calloc(sizeof(integer), 1);

	ret->len_of_integer = data->len_of_integer;
	ret->sign_of_integer = data->sign_of_integer;
	ret->vec_of_integer = (INTEGER_VECTOR) calloc(sizeof(INTEGER_HALF_TYPE) * data->len_of_integer, 1);
	memcpy(ret->vec_of_integer, data->vec_of_integer, sizeof(INTEGER_HALF_TYPE) * data->len_of_integer);
   return ret;
}
