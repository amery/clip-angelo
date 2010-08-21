/***********************************************/
integer *
integer_long_suba(integer * data, long y)
{
   integer *z = integer_long_init(y);

   integer_suba(data, z);
   integer_destroy(z);
   return data;
}
