/****************************************************/
integer *
integer_ssub(integer * d1, integer * d2)
{
	int k, i = d2->len_of_integer;

   INTEGER_FULL_TYPE sum = 1;

   for (k = 0; k < i; k++)
   {
		sum += INTEGER_MAXNUM - d2->vec_of_integer[k];
		sum += d1->vec_of_integer[k];
		d1->vec_of_integer[k] = EXTRACT(sum);
      sum >>= INTEGER_HALF;
   }
   return d1;
}
