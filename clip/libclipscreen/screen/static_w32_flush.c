static void
w32_flush(void)
{
   SMALL_RECT rect;

   int r;

   rect.Top = w32_beg.Y;
   rect.Left = w32_beg.X;
   rect.Bottom = w32_end.Y;
   rect.Right = w32_end.X;

   if (rect.Top > rect.Bottom || rect.Left > rect.Right)
      return;

   r = WriteConsoleOutput(w32_hStdOut, w32_screen, w32_size, w32_beg, &rect);

   w32_beg = w32_size;
   w32_end.X = 0;
   w32_end.Y = 0;
}
