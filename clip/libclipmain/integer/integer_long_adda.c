integer *
integer_long_adda(integer * data, long y)
{
   integer *z = integer_long_init(y);

   integer_adda(data, z);
   integer_destroy(z);
   return data;
}
