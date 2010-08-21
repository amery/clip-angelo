/**********************************************/
integer *
integer_mul(integer * d1, integer * d2)
{
   integer *ret = integer_copy(d1);

   integer_mula(ret, d2);
   return ret;
}
