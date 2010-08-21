/*********************************************/
int
integer_getBit(integer * data, int ind)
{
   int bw, sw;

   if (ind <= 0)
      return 0;
   bw = ind / INTEGER_HALF;
   sw = ind % INTEGER_HALF;
	return ((bw < data->len_of_integer && (data->vec_of_integer[bw] & (1 << sw))) != 0);
}
