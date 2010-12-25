integer *
integer_add(integer * d1, integer * d2)
{
   integer *ret = integer_copy(d1);

   integer_adda(ret, d2);
   return ret;
}
