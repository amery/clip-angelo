static void
w32_set_cursor_shape(int k)
{
   CONSOLE_CURSOR_INFO info;

   switch (k)
   {
   case 0:
      info.bVisible = 1;
      info.dwSize = 10;
      break;
   case 1:
      info.bVisible = 0;
      info.dwSize = 1;		/* ??? !!! $@!@#$% !!! */
      break;
   case 2:
      info.bVisible = 1;
      info.dwSize = 99;
      break;
   }
   SetConsoleCursorInfo(w32_hStdOut, &info);
}
