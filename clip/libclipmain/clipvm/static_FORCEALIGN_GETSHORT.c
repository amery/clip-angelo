static short
GETSHORT(void *p)
{
   short r;

   memcpy(&r, p, sizeof(r));
   return r;
}
