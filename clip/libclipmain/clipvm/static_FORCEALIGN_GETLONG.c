static long
GETLONG(void *p)
{
   long r;

   memcpy(&r, p, sizeof(r));
   return r;
}
