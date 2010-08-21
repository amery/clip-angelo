/***********************************************/
rational *
rational_diva(rational * data, rational * y)
{
   integer_mula(data->num, y->den);
   integer_mula(data->den, y->num);
   rational_normalize(data);
   return data;
}
