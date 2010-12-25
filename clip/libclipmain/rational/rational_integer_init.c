/*********************************************/
rational *
rational_integer_init(integer * y)
{
   rational *ret = (rational *) calloc(sizeof(rational), 1);

   ret->num = integer_copy(y);
   ret->den = integer_long_init(1L);
   return ret;
}
