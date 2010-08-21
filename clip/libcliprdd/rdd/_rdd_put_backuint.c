inline void
_rdd_put_backuint(unsigned char s[], unsigned int v)
{
   s[3] = v & 0x000000ff;
   v = v >> 8;
   s[2] = v & 0x000000ff;
   v = v >> 8;
   s[1] = v & 0x000000ff;
   v = v >> 8;
   s[0] = v & 0x000000ff;
}
