/**********************************************/
integer *
integer_lshift(integer * data, int y)
{
   integer *ret = integer_copy(data);

   integer_lshifta(ret, y);
   return ret;
}
