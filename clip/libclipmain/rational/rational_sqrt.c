/*********************************************/
rational *
rational_sqrt(rational * data)
{
   integer *tmp;

   rational *ret = rational_copy(data);

   if (integer_realLen(ret->den) < 3)
   {
      integer_lshifta(ret->num, 64L);
      integer_lshifta(ret->den, 64L);
   }

   tmp = integer_sqrt(ret->num);
   ret->num = integer_copy(tmp);
   integer_destroy(tmp);

   tmp = integer_sqrt(ret->den);
   ret->den = integer_copy(tmp);
   integer_destroy(tmp);

   rational_normalize(ret);
   return ret;
}
