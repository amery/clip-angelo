/*********************************************/
rational *
rational_long_init(long y)
{
   rational *ret = (rational *) calloc(sizeof(rational), 1);

   ret->num = integer_long_init(y);
   ret->den = integer_long_init(1L);
   return ret;
}
