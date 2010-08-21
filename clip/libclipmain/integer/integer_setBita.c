/*********************************************/
integer *
integer_setBita(integer * data, int ind, int value)
{
   int bw, sw;

   if (ind < 0)
      return data;
   bw = ind / INTEGER_HALF;
   sw = ind % INTEGER_HALF;
	if (value && data->len_of_integer <= bw)
      integer_resize(data, bw + 1);
	if (data->len_of_integer > bw)
		data->vec_of_integer[bw] |= ((unsigned int) value << sw);
   return data;
}
