inline unsigned int
_rdd_uint(unsigned char *s)
{
   unsigned int r;

   r = *(s + 3);
   r = r << 8;
   r += *(s + 2);
   r = r << 8;
   r += *(s + 1);
   r = r << 8;
   r += *s;
   return r;
}
