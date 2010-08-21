/**********************************************/
int
rational_cmp(rational * data, rational * y)
{
   integer *tmp1, *tmp2;

	int xsgn = data->num->sign_of_integer;

	int ysgn = y->num->sign_of_integer;

   int d = xsgn - ysgn;

   if (xsgn && !ysgn)
      return -1;
   if (!xsgn && ysgn)
      return 1;
   tmp1 = integer_copy(data->num);
   integer_mula(tmp1, y->den);
   tmp2 = integer_copy(data->den);
   integer_mula(tmp2, y->num);
   d = integer_cmp(tmp1, tmp2);

   integer_destroy(tmp1);
   integer_destroy(tmp2);

   return d;
}
