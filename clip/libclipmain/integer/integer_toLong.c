/*********************************************/
long
integer_toLong(integer * data)
{
	long ret = data->vec_of_integer[1] * INTEGER_MAXNUM | data->vec_of_integer[0];

	if (data->sign_of_integer)
      ret = -ret;
   return ret;
}
