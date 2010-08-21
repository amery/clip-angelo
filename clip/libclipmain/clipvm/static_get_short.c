static short
get_short(void *ptr)
{
   short **pp = (short **) ptr;

   return *(*pp)++;
}
