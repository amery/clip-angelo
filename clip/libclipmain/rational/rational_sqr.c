/*********************************************/
rational *
rational_sqr(rational * data)
{
   rational *ret = rational_copy(data);

   integer_mula(ret->num, data->num);
   integer_mula(ret->den, data->den);
   rational_normalize(ret);
   return ret;
}
