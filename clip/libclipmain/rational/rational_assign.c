/**********************************************/
rational *
rational_assign(rational * data, rational * y)
{
   integer_destroy(data->num);
   integer_destroy(data->den);
   data->num = integer_copy(y->num);
   data->den = integer_copy(y->den);
   rational_normalize(data);
   return data;
}
