/**********************************************/
integer *
integer_resize(integer * data, int newlen)
{
   newlen = newlen > 0 ? newlen : 1;
	data->vec_of_integer = realloc((void *) data->vec_of_integer, sizeof(INTEGER_HALF_TYPE) * newlen);
	if (data->len_of_integer < newlen)
		memset((void *) (data->vec_of_integer + data->len_of_integer), 0, sizeof(INTEGER_HALF_TYPE) * (newlen - data->len_of_integer));
	data->len_of_integer = newlen;
   return data;
}
