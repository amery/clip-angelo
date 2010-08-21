/*********************************************/
rational *
rational_pow(rational * data, long y)
{
   rational *ret = rational_copy(data);

   rational_powa(ret, y);
   return ret;
}
