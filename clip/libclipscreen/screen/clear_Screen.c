void
clear_Screen(Screen * scr)
{
   int i, j;

   ScreenData *dp = (ScreenData *) scr->base->data;

   int Lines = scr->base->Lines;

   int Columns = scr->base->Columns;

   for (i = 0; i < Lines; ++i)
      for (j = 0; j < Columns; ++j)
      {
	 scr->chars[i][j] = scr->base->realScreen->chars[i][j] = ' ';
	 scr->colors[i][j] = scr->base->realScreen->colors[i][j] = COLOR_WHITE | COLOR_BACK_BLACK;
	 scr->attrs[i][j] = scr->base->realScreen->attrs[i][j] = 0;
      }

#ifdef _WIN32
   if (w32_console)
   {
      w32_clear();
   }
   else
#endif
   {
      termcap_set_color(dp, COLOR_WHITE | COLOR_BACK_BLACK);
      termcap_clear_screen(dp);
      termcap_flush(dp);
   }
}
