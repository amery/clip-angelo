/***********************************************/
integer *
integer_lshifta(integer * data, int y)
{
   int bw, sw, rl, i, j;

   INTEGER_FULL_TYPE a = 0;

	if (y == 0 || data->len_of_integer == 0)
      return data;
   bw = y / INTEGER_HALF;
   sw = y % INTEGER_HALF;
   y = (y > 0 ? y : -y);
	rl = bw + data->len_of_integer + 1;
	j = data->len_of_integer;
	if (rl > data->len_of_integer)
      integer_resize(data, rl);
   for (rl--, i = j - 1; i >= 0; i--, rl--)
   {
		a = UP(a) | ((INTEGER_FULL_TYPE) data->vec_of_integer[i]) << sw;
		data->vec_of_integer[rl] = EXTRACT(DOWN(a));
   }
      data->vec_of_integer[rl] = EXTRACT(a);
   if (rl > 0)
		memset(data->vec_of_integer, 0, rl * sizeof(INTEGER_HALF_TYPE));
   return data;
}
