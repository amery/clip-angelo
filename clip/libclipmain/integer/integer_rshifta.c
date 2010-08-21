/***********************************************/
integer *
integer_rshifta(integer * data, int y)
{
   int bw, sw, rw, rl, i, j;

   INTEGER_FULL_TYPE a;

	if (y == 0 || data->len_of_integer == 0)
      return data;
   y = (y > 0 ? y : -y);
   bw = y / INTEGER_HALF;
   sw = y % INTEGER_HALF;
   rw = INTEGER_HALF - sw;
	rl = data->len_of_integer - bw;
   if (rl < 0)
   {
		memset(data->vec_of_integer, 0, data->len_of_integer * sizeof(INTEGER_HALF_TYPE));
      return data;
   }
      a = data->vec_of_integer[bw] >> sw;
		for (j = 0, i = bw + 1; i < data->len_of_integer; i++, j++)
   {
		a |= ((INTEGER_FULL_TYPE) data->vec_of_integer[i]) << rw;
		data->vec_of_integer[j] = EXTRACT(a);
      a >>= INTEGER_HALF;
   }
      data->vec_of_integer[j] = EXTRACT(a);
   integer_resize(data, j + 1);
   return data;
}
