inline void
_rdd_put_ushort(unsigned char s[], unsigned short int v)
{
   s[0] = v & 0x00ff;
   s[1] = (v & 0xff00) >> 8;
}
