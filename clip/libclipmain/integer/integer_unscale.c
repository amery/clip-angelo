/********************************************************/
integer *
integer_unscale(integer * data, INTEGER_HALF_TYPE y, INTEGER_HALF_TYPE * ost)
{
   integer *ret = integer_copy(data);

   integer_sunscale(ret, y, ost);
   return ret;
}
