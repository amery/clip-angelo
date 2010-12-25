static void
disp_box(ClipMachine * ClipMachineMemory, int Top, int Left, int Bottom, int Right, unsigned char *chars, int cl, int chars_n, char *color, int fill)
{
   Screen *sp;

   int attr = get_color(ClipMachineMemory, color);

   int i, j;

   int top, left, bottom, right;

   int ulc, urc, hlt, hlb, llc, lrc, vll, vlr, mch = ' ', pg_char = 0;

   _clip_fullscreen(ClipMachineMemory);
   sp = ClipMachineMemory->screen;

   if (!chars)
      pg_char = 1;

   if (!chars && chars_n == 2)
   {
      ulc = PGCH_ULCORNER2;
      urc = PGCH_URCORNER2;
      hlt = PGCH_HLINE2;
      hlb = PGCH_HLINE2;
      llc = PGCH_LLCORNER2;
      lrc = PGCH_LRCORNER2;
      vll = PGCH_VLINE2;
      vlr = PGCH_VLINE2;
   }
   else
   {
      ulc = PGCH_ULCORNER;
      urc = PGCH_URCORNER;
      hlt = PGCH_HLINE;
      hlb = PGCH_HLINE;
      llc = PGCH_LLCORNER;
      lrc = PGCH_LRCORNER;
      vll = PGCH_VLINE;
      vlr = PGCH_VLINE;
   }

   top = Top;
   left = Left;
   bottom = Bottom;
   right = Right;

   wind_region(ClipMachineMemory, &Top, &Left, &Bottom, &Right, 0);
   clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 0, -1);

   if (chars && cl > 0)
   {
      ulc = mch;
      urc = mch;
      hlt = mch;
      hlb = mch;
      llc = mch;
      lrc = mch;
      vll = mch;
      vlr = mch;
      if (cl > 0)
	 ulc = chars[0];
      if (cl > 1)
	 hlt = chars[1];
      if (cl > 2)
	 urc = chars[2];
      if (cl > 3)
	 vlr = chars[3];
      if (cl > 4)
	 lrc = chars[4];
      if (cl > 5)
	 hlb = chars[5];
      if (cl > 6)
	 llc = chars[6];
      if (cl > 7)
	 vll = chars[7];
      if (cl > 8)
      {
	 mch = chars[8];
	 fill = 1;
      }
   }

   for (i = top; i <= bottom; ++i)
   {
      sp->touched[i] = 1;
      for (j = left; j <= right; ++j)
      {
	 int ch;

	 if (top == bottom)
	    ch = hlt;
	 else if (left == right)
	    ch = vll;
	 else if (i == Top)
	 {
	    if (j == Left)
	       ch = ulc;
	    else if (j == Right)
	       ch = urc;
	    else
	       ch = hlt;
	 }
	 else if (i == Bottom)
	 {
	    if (j == Left)
	       ch = llc;
	    else if (j == Right)
	       ch = lrc;
	    else
	       ch = hlb;
	 }
	 else if (j == Left)
	    ch = vll;
	 else if (j == Right)
	    ch = vlr;
	 else if (fill)
	    ch = mch;
	 else
	    continue;

	 sp->chars[i][j] = ch;
	 /*if (pg_char)
	    sp->attrs[i][j] |= PG_ATTR; */
	 sp->colors[i][j] = attr;
      }
   }

   sync_mp(ClipMachineMemory);
}
