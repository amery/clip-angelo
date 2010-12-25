/*
 * Many other routines do put_queue, but I think either
 * they produce ASCII, or they produce some user-assigned
 * string, and in both cases we might assume that it is
 * in utf-8 already.
 */
static void
to_utf8(unsigned short c)
{
   if (c < 0x80)
      put_queue(c);		/*  0*******  */
   else if (c < 0x800)
   {
      put_queue(0xc0 | (c >> 6));	/*  110***** 10******  */
      put_queue(0x80 | (c & 0x3f));
   }
   else
   {
      put_queue(0xe0 | (c >> 12));	/*  1110**** 10****** 10******  */
      put_queue(0x80 | ((c >> 6) & 0x3f));
      put_queue(0x80 | (c & 0x3f));
   }
   /* UTF-8 is defined for words of up to 31 bits,
      but we need only 16 bits here */
}
