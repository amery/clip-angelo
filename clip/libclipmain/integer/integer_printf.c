/***********************************************/
void
integer_printf(char *str, integer * d)
{
   int i;

   printf(str);
	printf(": sign=%d, len=%d, data=", d->sign_of_integer, d->len_of_integer);
	for (i = 0; i < d->len_of_integer; i++)
		printf("%d", d->vec_of_integer[i]);
   printf("\n");
}
