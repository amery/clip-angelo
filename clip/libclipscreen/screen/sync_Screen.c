void
sync_Screen(Screen * scr, int utf8term)
{
   int y;

   int line, n, topline, botline;

   int Lines = scr->base->Lines;

   ScreenData *dp = (ScreenData *) scr->base->data;

/* make scrolls */

#ifdef _WIN32
   if (w32_console)
   {
      w32_beg = w32_size;
      w32_end.X = 0;
      w32_end.Y = 0;
   }
   else
#endif
   {

      for (line = 0; line < Lines; line++)
      {
	 /* find next range to scrool */

	 /* skip fresh lines */
	 while (line < Lines && scr->lnums[line] < 0)
	    line++;

	 /* last line reached - no range to scrool */
	 if (line >= Lines)
	    break;

	 /* top line found */
	 topline = line;

	 /* skip range of old lines */
	 while (line < Lines - 1 && scr->lnums[line] + 1 == scr->lnums[line + 1])
	    line++;

	 /* bottom line found */
	 botline = line;

	 /* compute number of scrools, >0 - forward */
	 n = topline - scr->lnums[topline];

	 if (n == 0)
	    continue;
	 else if (n > 0)
	    topline = scr->lnums[topline];
	 else if (n < 0)
	    botline = scr->lnums[botline];

	 /* do scrool */
	 termcap_scroll(scr->base, topline, botline, n);
      }

   }
   for (line = 0; line < Lines; line++)
      scr->lnums[line] = line;

   if (scr->base->mouse_present && !xterm_mouse && scr->base->mouse_visible && scr->base->mouse_auto_visible)
   {
      unsigned char *ptr;

      ptr = scr->colors[scr->base->mouse_y] + scr->base->mouse_x;
      *ptr = ~*ptr;
      scr->touched[scr->base->mouse_y] = 1;
   }

/* update contens */
   for (y = 0; y < Lines; y++)
      syncLine(scr, y, utf8term);

#ifdef _WIN32
   if (w32_console)
   {
      for (; scr->beeps > 0; --scr->beeps)
	 w32_beep();
      w32_set_cursor(scr->y, scr->x);
      w32_set_cursor_shape(scr->cursor);
      w32_flush();
   }
   else
#endif
   {
      for (; scr->beeps > 0; --scr->beeps)
	 termcap_beep(dp);

      termcap_set_cursor(dp, scr->y, scr->x);
      termcap_set_cursor_shape(dp, scr->cursor);

      termcap_flush(dp);
   }

   if (scr->base->mouse_present && !xterm_mouse && scr->base->mouse_visible && scr->base->mouse_auto_visible)
   {
      unsigned char *ptr;

      ptr = scr->colors[scr->base->mouse_y] + scr->base->mouse_x;
      *ptr = ~*ptr;
      scr->touched[scr->base->mouse_y] = 1;
   }
}
