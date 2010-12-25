/**********************************************/
integer *
integer_rshift(integer * data, int y)
{
   integer *ret = integer_copy(data);

   integer_rshifta(ret, y);
   return ret;
}
