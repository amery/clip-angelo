/***********************************************/
rational *
rational_mula(rational * data, rational * y)
{
   integer_mula(data->num, y->num);
   integer_mula(data->den, y->den);
   rational_normalize(data);
   return data;
}
