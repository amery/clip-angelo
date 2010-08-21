static void
w32_set_cursor(int r, int c)
{
   COORD coord;

   coord.X = c;
   coord.Y = r;
   SetConsoleCursorPosition(w32_hStdOut, coord);
}
