static unsigned long
read_ulong(char *src)
{
   return ((unsigned char) src[0])
    | (((unsigned char) src[1]) << 8) | (((unsigned char) src[2]) << 16) | (((unsigned char) src[3]) << 24);
}

