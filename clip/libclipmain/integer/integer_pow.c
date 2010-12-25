/*********************************************/
integer *
integer_pow(integer * data, long y)
{
   integer *ret = integer_copy(data);

   integer_powa(ret, y);
   return ret;
}
