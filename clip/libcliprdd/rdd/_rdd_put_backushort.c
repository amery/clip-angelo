inline void
_rdd_put_backushort(unsigned char s[], unsigned short int v)
{
   s[1] = v & 0x00ff;
   s[0] = (v & 0xff00) >> 8;
}
