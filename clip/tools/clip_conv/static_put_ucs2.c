static void
put_ucs2(unsigned long wc)
{
   putchar((wc >> 8) & 0xff);
   putchar(wc & 0xff);
}
