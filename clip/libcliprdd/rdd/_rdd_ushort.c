inline unsigned short int
_rdd_ushort(unsigned char *s)
{
   unsigned short int r;

   r = *(s + 1);
   r = r << 8;
   r += *s;
   return r;
}
