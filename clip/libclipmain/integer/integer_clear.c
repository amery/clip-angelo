/***********************************************/
integer *
integer_clear(integer * data)
{
	data->sign_of_integer = 0;
	memset(data->vec_of_integer, 0, data->len_of_integer * sizeof(INTEGER_HALF_TYPE));
   return data;
}
