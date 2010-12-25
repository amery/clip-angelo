/*********************************************/
rational *
rational_long2_init(long i, long j)
{
   rational *ret = (rational *) calloc(sizeof(rational), 1);

   ret->num = integer_long_init(i);
   ret->den = integer_long_init(j);
   rational_normalize(ret);
   return ret;
}
