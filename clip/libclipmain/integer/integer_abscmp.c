/***********************************************/
int
integer_abscmp(integer * d1, integer * d2)
{
   int i, j;

	for (i = d1->len_of_integer - 1; i >= 0 && d1->vec_of_integer[i] == 0; i--);
	for (j = d2->len_of_integer - 1; j >= 0 && d2->vec_of_integer[j] == 0; j--);
   if (i > j)
      return 1;
   if (i < j)
      return -1;
	for (; i >= 0 && d1->vec_of_integer[i] == d2->vec_of_integer[i]; i--);
   if (i < 0)
      return 0;
	if (d1->vec_of_integer[i] > d2->vec_of_integer[i])
      return 1;
   return -1;
}
