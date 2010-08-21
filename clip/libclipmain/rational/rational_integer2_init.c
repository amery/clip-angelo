/*********************************************/
rational *
rational_integer2_init(integer * y, integer * z)
{
   rational *ret = (rational *) calloc(sizeof(rational), 1);

   ret->num = integer_copy(y);
   ret->den = integer_copy(z);
   rational_normalize(ret);
   return ret;
}
