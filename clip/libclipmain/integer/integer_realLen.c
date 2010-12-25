int
integer_realLen(integer * data)
{
	int i = data->len_of_integer - 1;

	for (; i >= 0 && data->vec_of_integer[i] == 0; i--);
   return i + 1;
}
