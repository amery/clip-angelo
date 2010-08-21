/*********************************************/
rational *
rational_copy(rational * y)
{
   rational *ret = (rational *) calloc(sizeof(rational), 1);

   ret->num = integer_copy(y->num);
   ret->den = integer_copy(y->den);
   rational_normalize(ret);
   return ret;
}
