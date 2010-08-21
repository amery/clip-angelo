/***********************************************/
integer *
integer_sub(integer * data, integer * y)
{
   integer *ret = integer_copy(data);

   integer_suba(ret, y);
   return ret;
}
