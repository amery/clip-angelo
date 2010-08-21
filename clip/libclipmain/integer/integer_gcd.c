/*********************************************/
integer *
integer_gcd(integer * data, integer * y)
{
   integer *u = integer_copy(data);

   integer *v = NULL, *t = NULL;

   int rl, i, j, br = 0;

   int k = 0, m;

   INTEGER_FULL_TYPE a, b;

	for (i = data->len_of_integer - 1; i >= 0 && data->vec_of_integer[i] == 0; i--);
	for (j = y->len_of_integer - 1; j >= 0 && y->vec_of_integer[j] == 0; j--);
   if (j < 0)
      return u;
   if (i < 0)
   {
      integer_destroy(u);
      u = integer_copy(y);
      return u;
   }
   rl = j > i ? j : i;

   for (i = 0; i <= rl; i++)
   {
		a = data->vec_of_integer[i];
		b = y->vec_of_integer[i];
      for (j = 0; j < INTEGER_HALF; j++, k++)
      {
	 if ((a | b) & ((INTEGER_FULL_TYPE) 1))
	 {
	    br = 1;
	    break;
	 }
	 a >>= 1;
	 b >>= 1;
      }
      if (br)
	 break;
   }
   integer_rshifta(u, k);
   v = integer_rshift(y, k);

	u->sign_of_integer = 0;
	v->sign_of_integer = 0;
	if (u->vec_of_integer[0] & 1)
      t = integer_inv(v);
   else
      t = integer_copy(u);

   while (!integer_empty(t))
   {
      br = 0;
		for (i = 0, m = 0; i < t->len_of_integer; i++)
      {
			for (j = 0, a = t->vec_of_integer[i]; j < INTEGER_HALF; j++, m++, a >>= 1)
	 {
	    if (a & ((INTEGER_FULL_TYPE) 1))
	    {
	       br = 1;
	       break;
	    }
	 }
	 if (br)
	    break;
      }
      if (m > 0)
	 integer_rshifta(t, m);
	 if (t->sign_of_integer)
      {
	 integer_destroy(v);
	 v = integer_inv(t);
	 integer_adda(t, u);
      }
      else
      {
	 integer_destroy(u);
	 u = integer_copy(t);
	 integer_suba(t, v);
      }
   }
   integer_lshifta(u, k);
   integer_destroy(t);
   integer_destroy(v);
   return u;
}
