/****************************************************/
integer *
integer_sadd(integer * d1, integer * d2)
{
	int k, i = d2->len_of_integer;

   INTEGER_FULL_TYPE sum = 0;

	if (d1->vec_of_integer[d1->len_of_integer - 1] > (INTEGER_MAXNUM >> 1))
   {
		d1->len_of_integer++;
		d1->vec_of_integer = realloc(d1->vec_of_integer, d1->len_of_integer * sizeof(INTEGER_HALF_TYPE));
		d1->vec_of_integer[d1->len_of_integer - 1] = 0;
   }
   for (k = 0; k < i; k++)
   {
		sum += ((INTEGER_FULL_TYPE) (d1->vec_of_integer[k])) + ((INTEGER_FULL_TYPE) (d2->vec_of_integer[k]));
		d1->vec_of_integer[k] = EXTRACT(sum);
      sum >>= INTEGER_HALF;
   }
   for (; sum != 0; k++)
   {
		sum += d1->vec_of_integer[k];
		d1->vec_of_integer[k] = EXTRACT(sum);
      sum >>= INTEGER_HALF;
   }
   return d1;
}
