int
integer_empty(integer * data)
{
   int i;

	for (i = data->len_of_integer - 1; i >= 0 && data->vec_of_integer[i] == 0; i--);
   return (i < 0);
}
