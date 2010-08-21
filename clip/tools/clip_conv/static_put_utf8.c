static void
put_utf8(unsigned long wc)
{
   int count, i;

   unsigned char r[6];

   if (wc < 0x80)
      count = 1;
   else if (wc < 0x800)
      count = 2;
   else if (wc < 0x10000)
      count = 3;
   else if (wc < 0x200000)
      count = 4;
   else if (wc < 0x4000000)
      count = 5;
   else if (wc <= 0x7fffffff)
      count = 6;
   else
      return;
   switch (count)
   {				/* note: code falls through cases! */
   case 6:
      r[5] = 0x80 | (wc & 0x3f);
      wc = wc >> 6;
      wc |= 0x4000000;
   case 5:
      r[4] = 0x80 | (wc & 0x3f);
      wc = wc >> 6;
      wc |= 0x200000;
   case 4:
      r[3] = 0x80 | (wc & 0x3f);
      wc = wc >> 6;
      wc |= 0x10000;
   case 3:
      r[2] = 0x80 | (wc & 0x3f);
      wc = wc >> 6;
      wc |= 0x800;
   case 2:
      r[1] = 0x80 | (wc & 0x3f);
      wc = wc >> 6;
      wc |= 0xc0;
   case 1:
      r[0] = wc;
   }

   for (i = 0; i < count; i++)
      putchar(r[i]);
}
