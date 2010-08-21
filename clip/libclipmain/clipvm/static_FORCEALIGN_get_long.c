static long
get_long(void *ptr)
{
   long **pp = (long **) ptr;

   long r;

   memcpy(&r, pp, sizeof(r));
   (*pp)++;
   return r;
}
