/**********************************************/
integer *
integer_and(integer * d1, integer * d2)
{
   integer *ret = integer_copy(d1);

   integer_anda(ret, d2);
   return ret;
}
