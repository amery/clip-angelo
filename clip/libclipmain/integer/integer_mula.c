/**********************************************/
integer *
integer_mula(integer * d1, integer * d2)
{
   int a, b, i, j, k;

	int sgn = d1->sign_of_integer ^ d2->sign_of_integer;

   INTEGER_FULL_TYPE ai, sum;

	for (a = d1->len_of_integer - 1; a >= 0 && d1->vec_of_integer[a] == 0; a--);

   if (a < 0)
      return d1;
	for (b = d2->len_of_integer - 1; b >= 0 && d2->vec_of_integer[b] == 0; b--);
   if (b < 0)
   {
		memset(d1->vec_of_integer, 0, sizeof(INTEGER_HALF_TYPE) * d1->len_of_integer);
      return d1;
   }
      d1->sign_of_integer = sgn;
		if (a == 0 && d1->vec_of_integer[0] == 1)
   {
		if (d1->vec_of_integer != NULL)
			free(d1->vec_of_integer);
		d1->len_of_integer = d2->len_of_integer;
		d1->vec_of_integer = (INTEGER_VECTOR) calloc(d2->len_of_integer * sizeof(INTEGER_HALF_TYPE), 1);
		memcpy(d1->vec_of_integer, d2->vec_of_integer, d2->len_of_integer * sizeof(INTEGER_HALF_TYPE));
		d1->sign_of_integer = sgn;
      return d1;
   }
      if (b == 0 && d2->vec_of_integer[0] == 1)
      return d1;
   integer_resize(d1, a + b + 2);
   for (i = a; i >= 0; i--)
   {
		ai = d1->vec_of_integer[i];
		d1->vec_of_integer[i] = 0;
      if (ai == 0)
	 continue;
      sum = 0;
      for (j = 0, k = i; j <= b; j++, k++)
      {
			sum += ai * d2->vec_of_integer[j];
			sum += d1->vec_of_integer[k];
			d1->vec_of_integer[k] = EXTRACT(sum);
	 sum >>= INTEGER_HALF;
      }
            for (; sum != 0 && k < d1->len_of_integer; k++)
      {
			sum += d1->vec_of_integer[k];
			d1->vec_of_integer[k] = EXTRACT(sum);
	 sum >>= INTEGER_HALF;
      }
   }
   return d1;
}
