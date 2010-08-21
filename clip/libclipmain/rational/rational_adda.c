/*********************************************/
rational *
rational_adda(rational * data, rational * a)
{
   integer *tmp;

   tmp = integer_copy(a->num);
   integer_mula(data->num, a->den);
   integer_mula(data->den, a->den);
   integer_mula(tmp, data->den);
   integer_adda(data->num, tmp);
   integer_destroy(tmp);

   integer_mula(data->den, a->den);
   rational_normalize(data);
   return data;
}
