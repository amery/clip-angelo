/***********************************************/
rational *
rational_mul(rational * data, rational * y)
{
   rational *ret = rational_copy(data);

   rational_mula(ret, y);
   return ret;
}
