static short
get_short(void *ptr)
{
   short **pp = (short **) ptr;

   short r;

   memcpy(&r, pp, sizeof(r));
   (*pp)++;
   return r;
}
