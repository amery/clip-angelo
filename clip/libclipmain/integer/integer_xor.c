/**********************************************/
integer *
integer_xor(integer * d1, integer * d2)
{
   integer *ret = integer_copy(d1);

   integer_xora(ret, d2);
   return ret;
}
