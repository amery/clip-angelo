inline void
_rdd_put_uint(unsigned char s[], unsigned int v)
{
   s[0] = v & 0x000000ff;
   v = v >> 8;
   s[1] = v & 0x000000ff;
   v = v >> 8;
   s[2] = v & 0x000000ff;
   v = v >> 8;
   s[3] = v & 0x000000ff;
}
