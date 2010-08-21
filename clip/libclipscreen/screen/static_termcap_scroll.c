static void
termcap_scroll(ScreenBase * base, int top, int bottom, int n)
{
   ScreenData *dp = (ScreenData *) base->data;

   Screen *realScreen = base->realScreen;

   int Lines = base->Lines;

   int Columns = base->Columns;

   if (n > 0)
   {
      memmove(realScreen->chars[top + n], realScreen->chars[top], (bottom - top + 1 - n) * Columns);
      memset(realScreen->chars[top], ' ', n * Columns);
      memset(realScreen->colors[top], dp->oldcolor, n * Columns);
      memset(realScreen->attrs[top], 0, n * Columns);
      if (dp->termcap_SR && top == 0 && bottom == Lines - 1)
      {
	 termcap_set_cursor(dp, 0, 0);
	 while (--n >= 0)
	    termcap_put_raw_str(dp, dp->termcap_SR);
      }
      else if (dp->termcap_AL && dp->termcap_DL)
	 while (--n >= 0)
	 {
	    termcap_set_cursor(dp, bottom, 0);
	    termcap_put_raw_str(dp, dp->termcap_DL);
	    termcap_set_cursor(dp, top, 0);
	    termcap_put_raw_str(dp, dp->termcap_AL);
	 }
   }
   else
   {
      memmove(realScreen->chars[top], realScreen->chars[top - n], (bottom - top + 1 + n) * Columns);
      memset(realScreen->chars[bottom + n + 1], ' ', (-n) * Columns);
      memset(realScreen->colors[bottom + n + 1], dp->oldcolor, (-n) * Columns);
      memset(realScreen->attrs[bottom + n + 1], 0, (-n) * Columns);
      if (dp->termcap_SF && top == 0 && bottom == Lines - 1)
      {
	 termcap_set_cursor(dp, Lines - 1, 0);
	 while (++n <= 0)
	    termcap_put_raw_str(dp, dp->termcap_SF);
      }
      else if (dp->termcap_AL && dp->termcap_DL)
	 while (++n <= 0)
	 {
	    termcap_set_cursor(dp, top, 0);
	    termcap_put_raw_str(dp, dp->termcap_DL);
	    termcap_set_cursor(dp, bottom, 0);
	    termcap_put_raw_str(dp, dp->termcap_AL);
	 }
   }
}
