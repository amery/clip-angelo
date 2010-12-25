inline unsigned int
_rdd_backuint(unsigned char *s)
{
   unsigned int r;

   r = *s;
   r = r << 8;
   r += *(s + 1);
   r = r << 8;
   r += *(s + 2);
   r = r << 8;
   r += *(s + 3);
   return r;
}
