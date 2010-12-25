/**********************************************/
integer *
integer_or(integer * d1, integer * d2)
{
   integer *ret = integer_copy(d1);

   integer_ora(ret, d2);
   return ret;
}
