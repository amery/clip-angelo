/*********************************************/
rational *
rational_suba(rational * data, rational * a)
{
   integer *tmp;

   tmp = integer_copy(a->num);
   integer_mula(data->num, a->den);
   integer_mula(data->den, a->den);
   integer_mula(tmp, data->den);
   integer_suba(data->num, tmp);
   integer_destroy(tmp);

   rational_normalize(data);
   return data;
}
