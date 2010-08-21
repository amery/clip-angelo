int
u32toutf8(char *utf8, unsigned long uc32)
{
   int n;

   if (uc32 < 0x00000080L)
   {
      utf8[0] = uc32 & 0xff;
      n = 1;
   }
   else if (uc32 < 0x00000800L)
   {
      utf8[0] = 0xc0 | ((uc32 >> 6) & 0x1f);
      utf8[1] = 0x80 | (uc32 & 0x3f);
      n = 2;
   }
   else if (uc32 < 0x00010000L)
   {
      utf8[0] = 0xe0 | ((uc32 >> 12) & 0x0f);
      utf8[1] = 0x80 | ((uc32 >> 6) & 0x3f);
      utf8[2] = 0x80 | (uc32 & 0x3f);
      n = 3;
   }
   else if (uc32 < 0x00200000L)
   {
      utf8[0] = 0xf0 | ((uc32 >> 18) & 0x07);
      utf8[1] = 0x80 | ((uc32 >> 12) & 0x3f);
      utf8[2] = 0x80 | ((uc32 >> 6) & 0x3f);
      utf8[3] = 0x80 | (uc32 & 0x3f);
      n = 4;
   }
   else if (uc32 < 0x04000000L)
   {
      utf8[0] = 0xf8 | ((uc32 >> 24) & 0x03);
      utf8[1] = 0x80 | ((uc32 >> 18) & 0x3f);
      utf8[2] = 0x80 | ((uc32 >> 12) & 0x3f);
      utf8[3] = 0x80 | ((uc32 >> 6) & 0x3f);
      utf8[4] = 0x80 | (uc32 & 0x3f);
      n = 5;
   }
   else if (uc32 < 0x80000000L)
   {
      utf8[0] = 0xfc | ((uc32 >> 30) & 0x01);
      utf8[1] = 0x80 | ((uc32 >> 24) & 0x3f);
      utf8[2] = 0x80 | ((uc32 >> 18) & 0x3f);
      utf8[3] = 0x80 | ((uc32 >> 12) & 0x3f);
      utf8[4] = 0x80 | ((uc32 >> 6) & 0x3f);
      utf8[5] = 0x80 | (uc32 & 0x3f);
      n = 6;
   }
   else
   {
      n = 0;
   }
   return n;
}
