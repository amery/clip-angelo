void
loc_write(DbfLocale * loc, unsigned char *buf, int len)
{
   unsigned char *c = buf;

   unsigned char *e = c + len;

   for (; c < e; c++)
      if (*c & 0x80)
	 *c = loc->write[*c & 0x7f];
}
