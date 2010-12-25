/**********************************************/
integer *
integer_xora(integer * d1, integer * d2)
{
   int i;

	if (d1->len_of_integer < d2->len_of_integer)
		integer_resize(d1, d2->len_of_integer);
	for (i = 0; i < d1->len_of_integer; i++)
		d1->vec_of_integer[i] ^= d2->vec_of_integer[i];
   return d1;
}
