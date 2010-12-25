/***********************************************/
integer *
integer_long_mula(integer * data, long y)
{
   integer *z = integer_long_init(y);

   integer_mula(data, z);
   integer_destroy(z);
   return data;
}
