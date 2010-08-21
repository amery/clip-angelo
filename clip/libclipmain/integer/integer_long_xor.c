/**********************************************/
integer *
integer_long_xor(integer * d1, long y)
{
   integer *d2 = integer_long_init(y);

   integer *ret = integer_copy(d1);

   integer_xora(ret, d2);
   integer_destroy(d2);
   return ret;
}
