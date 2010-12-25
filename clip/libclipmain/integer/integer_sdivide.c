/****************************************************/
integer *
integer_sdivide(integer * data, integer * y, integer ** div)
{
   int rl1, rl2, cmp, i;

   INTEGER_HALF_TYPE ost;

   integer *var = NULL;

	for (rl1 = data->len_of_integer - 1; rl1 >= 0 && data->vec_of_integer[rl1] == 0; rl1--);
	for (rl2 = y->len_of_integer - 1; rl2 >= 0 && y->vec_of_integer[rl2] == 0; rl2--);
   cmp = integer_abscmp(data, y);
   if (rl1 < 0 || rl2 < 0 || rl1 < rl2 || cmp < 0)
   {
      integer_resize(data, 1);
		data->vec_of_integer[0] = 0;
		data->len_of_integer = 1;
      return data;
   }
   if (cmp == 0)
   {
      integer_resize(data, 1);
		data->vec_of_integer[0] = 1;
		data->len_of_integer = 1;
      return data;
   }
            data->sign_of_integer = data->sign_of_integer ^ y->sign_of_integer;
   if (rl2 == 0)
   {
		integer_sunscale(data, y->vec_of_integer[0], &ost);
      (*div) = integer_long_init(ost);
      return data;
   }
   (*div) = integer_copy(data);
	(*div)->sign_of_integer = 1;
   var = integer_copy(y);
	var->sign_of_integer = 0;
   i = (rl1 - rl2) * INTEGER_HALF;
   if (i > INTEGER_HALF)
      integer_lshifta(var, i);
   else
      i = 0;
   for (; integer_abscmp(*div, var) > 0; i++)
      integer_lshifta(var, 1);
   for (; integer_abscmp(*div, var) < 0; i--)
      integer_rshifta(var, 1);
   integer_resize(data, (i / INTEGER_HALF) + 1);
	memset(data->vec_of_integer, 0, data->len_of_integer * sizeof(INTEGER_HALF_TYPE));
	rl2 = var->len_of_integer - 1;
	rl1 = (*div)->len_of_integer - 1;
   for (; i >= 0; i--)
   {
      cmp = integer_abscmp(*div, var);
      if (cmp >= 0)
      {
			data->vec_of_integer[i / INTEGER_HALF] |= (1 << (INTEGER_HALF_TYPE) (i % INTEGER_HALF));
	 integer_ssub(*div, var);
      }
      if (cmp == 0)
	 break;			// ���� ����� �����
      integer_rshifta(var, 1);
   }
   integer_destroy(var);
   return data;
}
