/***********************************************/
integer *
integer_long_add(integer * data, long y)
{
   integer *ret = integer_copy(data);

   integer *z = integer_long_init(y);

   integer_adda(ret, z);
   integer_destroy(z);
   return ret;
}
