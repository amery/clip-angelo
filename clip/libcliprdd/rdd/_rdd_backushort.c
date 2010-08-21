inline unsigned short int
_rdd_backushort(unsigned char *s)
{
   unsigned short int r;

   r = *s;
   r = r << 8;
   r += *(s + 1);
   return r;
}
