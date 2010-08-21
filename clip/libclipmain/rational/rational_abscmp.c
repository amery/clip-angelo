/**********************************************/
int
rational_abscmp(rational * data, rational * y)
{
   int ret;

   integer *tmp1, *tmp2;

   tmp1 = integer_copy(data->num);
   integer_mula(tmp1, y->den);
   tmp2 = integer_copy(data->den);
   integer_mula(tmp1, y->num);
   ret = integer_abscmp(tmp1, tmp2);
   integer_destroy(tmp1);
   integer_destroy(tmp2);
   return ret;
}
