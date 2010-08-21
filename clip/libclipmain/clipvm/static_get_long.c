static long
get_long(void *ptr)
{
   long **pp = (long **) ptr;

   return *(*pp)++;
}
