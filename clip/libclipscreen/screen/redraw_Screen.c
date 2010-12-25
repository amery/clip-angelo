void
redraw_Screen(Screen * scr, int utf8term)
{
   int i, j;

   int Lines;

   int Columns;

   ScreenData *dp;

   unsigned char **chars;

   unsigned char **colors;

   unsigned char **attrs;

   if (!scr)
      return;

   Lines = scr->base->Lines;
   Columns = scr->base->Columns;
   dp = (ScreenData *) scr->base->data;
   chars = scr->base->realScreen->chars;
   colors = scr->base->realScreen->colors;
   attrs = scr->base->realScreen->attrs;

   for (i = 0; i < Lines; ++i)
   {
      for (j = 0; j < Columns; ++j)
      {
	 chars[i][j] = ' ';
	 colors[i][j] = COLOR_WHITE | COLOR_BACK_BLACK;
	 attrs[i][j] = 0;
      }
      scr->touched[i] = 1;
   }

   termcap_set_color(dp, COLOR_WHITE | COLOR_BACK_BLACK);
   termcap_clear_screen(dp);
   termcap_flush(dp);

   sync_Screen(scr, utf8term);

}
