/*********************************************/
int
integer_lg(integer * data)
{
   int rl, ret;

   INTEGER_HALF_TYPE a;

	for (rl = data->len_of_integer - 1; rl >= 0 && data->vec_of_integer[rl] == 0; rl--);
   if (rl < 0)
      return 0;
	a = data->vec_of_integer[rl];
   for (ret = (rl) * INTEGER_HALF - 1; a != 0; ret++)
      a >>= 1;
   return ret;
}
