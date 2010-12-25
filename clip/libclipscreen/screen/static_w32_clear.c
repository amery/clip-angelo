static void
w32_clear(void)
{
   CHAR_INFO p;

   int i;

   p.Char.AsciiChar = ' ';
   p.Attributes = 7;
   for (i = 0; i < w32_scrsize; i++)
      w32_screen[i] = p;
   w32_beg.X = 0;
   w32_beg.Y = 0;
   w32_end.X = w32_size.X - 1;
   w32_end.Y = w32_size.Y - 1;
   w32_flush();
}
