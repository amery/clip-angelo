static void
translate(unsigned char *str, int len, int up)
{
   unsigned char *tbl = (up ? _clip_uptbl : _clip_lowtbl);

   while (len)
   {
      int s = *str;

      *str = tbl[s];
      --len;
      ++str;
   }
}
