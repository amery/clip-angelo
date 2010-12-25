void
loc_read(DbfLocale * loc, unsigned char *buf, int len)
{
   unsigned char *c = buf;

   unsigned char *e = c + len;

   for (; c < e; c++)
      if (*c & 0x80)
	 *c = loc->read[*c & 0x7f];
}
