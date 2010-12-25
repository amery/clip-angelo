static void
syncLine(Screen * scr, int y, int utf8term)
{
   int Lines = scr->base->Lines;

   int Columns = scr->base->Columns;

   int columns = (y == Lines - 1 ? Columns - 1 : Columns);

   int x, e, i, end, l, contnum;

   unsigned char *chars, *ochars, *colors, *ocolors, *attrs, *oattrs;

   ScreenData *dp = (ScreenData *) scr->base->data;

   if (!scr->touched[y])
      return;
   scr->touched[y] = 0;

   chars = scr->chars[y];
   ochars = scr->base->realScreen->chars[y];
   colors = scr->colors[y];
   ocolors = scr->base->realScreen->colors[y];
   attrs = scr->attrs[y];
   oattrs = scr->base->realScreen->attrs[y];

   for (x = 0; x < columns && chars[x] == ochars[x] && colors[x] == ocolors[x] && attrs[x] == oattrs[x]; x++);

   if (x >= columns)
      return;

   for (l = columns - 1; l >= x && chars[l] == ochars[l] && colors[l] == ocolors[l] && attrs[l] == oattrs[l]; l--);

   if (l < x)
      return;

   e = end = l;
   i = x;

#ifdef _WIN32
   if (w32_console)
   {
      CHAR_INFO *p;

      if (y < w32_beg.Y)
	 w32_beg.Y = y;
      if (y > w32_end.Y)
	 w32_end.Y = y;
      if (x < w32_beg.X)
	 w32_beg.X = x;
      if (e > w32_end.X)
	 w32_end.X = e;

      for (p = w32_screen + y * w32_size.X + x; i <= e; i++, p++)
      {
	 int ch;

	 /* int pg; */

	 ch = chars[i];
	 p->Attributes = colors[i];
/*
			if (dp->pg_mode && ch < 32 && (pg = dp->pgTable[ch]))
				ch = dp->base->pg_chars[pg];
			else
*/
	 ch = dp->outputTable[ch];
	 p->Char.AsciiChar = ch;

	 ocolors[i] = colors[i];
	 ochars[i] = chars[i];
	 oattrs[i] = attrs[i];
      }

      return;
   }
#endif

/* Search the first nonblank character from the end of line. */
   if (dp->termcap_CE)
   {
      int p = columns - 1;

      if (chars[p] == ' ')
	 while (p > 0 && chars[p] == chars[p - 1] && colors[p] == colors[p - 1] && attrs[p] == attrs[p - 1])
	    --p;
      /* If there are more than 4 characters to clear, use CE. */
      if (e > p + 4 || (e > p && y >= Lines - 1))
	 e = p - 1;
   }

   contnum = 0;
   for (; i <= e; i++, x++)
   {
      if (x >= Columns || y >= Lines)
	 return;

      if (chars[i] == ochars[i] && colors[i] == ocolors[i] && attrs[i] == oattrs[i])
      {
	 contnum = 0;
	 continue;
      }

      if (!contnum)
	 termcap_set_cursor(dp, y, x);
      contnum++;
      termcap_set_color(dp, colors[i] /*& 0x7f */ );
      /*if (colors[i] & 0x80) */
      if (attrs[i] & PG_ATTR)
	 termcap_put_graph_char(dp, chars[i]);
      else
	 termcap_put_char(dp, chars[i], utf8term);
      ocolors[i] = colors[i];
      ochars[i] = chars[i];
      oattrs[i] = attrs[i];
   }

/* Clear the end of line.
   First check if it is needed. */

   if (dp->termcap_CE && e < end)
   {
      termcap_set_cursor(dp, y, x);
      termcap_set_color(dp, colors[i]);
      termcap_put_raw_str(dp, dp->termcap_CE);
      while (i <= end)
      {
	 ocolors[i] = colors[i];
	 ochars[i] = chars[i];
	 oattrs[i] = attrs[i];
	 i++;
      }
   }

}
