static void
write_ulong(char *dst, unsigned long l)
{
   dst[0] = l & 0xff;
   dst[1] = (l >> 8) & 0xff;
   dst[2] = (l >> 16) & 0xff;
   dst[3] = (l >> 24) & 0xff;
}

