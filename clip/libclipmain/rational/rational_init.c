/*********************************************/
rational *
rational_init()
{
   rational *ret = (rational *) calloc(sizeof(rational), 1);

   ret->num = integer_long_init(0L);
   ret->den = integer_long_init(1L);
   return ret;
}
