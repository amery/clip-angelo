static int
term_match_Key(ScreenBase * base, unsigned char b, unsigned long *keyp)
{
   int trymatch = 0;

   unsigned char kb;

   unsigned long key = b;

   ScreenData *dp = (ScreenData *) base->data;

   Keytab *keytab = dp->keytab;

   int key_count = dp->key_count;

   if (xterm_mouse && xterm_pos)
   {
      int mb, t, bb = 0;
      static struct timeval tv1 = { 0, 0 }, tv2;
      static int clicks = 0;

      switch (xterm_pos)
      {
      case 1:
	 xterm_buttons = b - 040;
	 xterm_pos = 2;
	 return 0;
      case 2:
	 xterm_x = b - 040 - 1;
	 xterm_pos = 3;
	 return 0;
      case 3:
      default:
	 xterm_y = b - 040 - 1;
	 xterm_pos = 0;

	 if (xterm_x < base->mouse_left)
	    xterm_x = base->mouse_left;
	 if (xterm_x > base->mouse_right)
	    xterm_x = base->mouse_right;
	 if (xterm_y < base->mouse_top)
	    xterm_y = base->mouse_top;
	 if (xterm_y > base->mouse_bottom)
	    xterm_y = base->mouse_bottom;
	 base->mouse_x = xterm_x;
	 base->mouse_y = xterm_y;
	 base->mouse_auto_visible = 1;

	 mb = xterm_buttons & 0x3;
	 if (mb != 3)
	 {
	    long dt;

	    if (mb == 0)
	       bb = MOUSE_BUTTONS_LEFT;
	    else if (mb == 1)
	       bb = MOUSE_BUTTONS_MIDDLE;
	    else if (mb == 2)
	       bb = MOUSE_BUTTONS_RIGHT;
	    t = MOUSE_TYPE_DOWN;
	    base->mouse_buttons = bb;

	    gettimeofday(&tv2, 0);

	    dt = (tv2.tv_sec - tv1.tv_sec) * 1000 + (tv2.tv_usec - tv1.tv_usec) / 1000;

	    if (tv1.tv_sec && dt < base->mouse_dclick_speed)
	    {
	       clicks++;
	       clicks %= 2;
	    }
	    else
	       clicks = 0;
	 }
	 else
	 {
	    bb = base->mouse_buttons;
	    if (bb == MOUSE_BUTTONS_LEFT)
	       base->mouse_clicks_left++;
	    if (bb == MOUSE_BUTTONS_RIGHT)
	       base->mouse_clicks_right++;
	    if (bb == MOUSE_BUTTONS_MIDDLE)
	       base->mouse_clicks_middle++;
	    base->mouse_buttons = 0;
	    t = MOUSE_TYPE_UP | (MOUSE_TYPE_SINGLE << clicks);
	    gettimeofday(&tv1, 0);
	    clicks = 0;
	 }

	 _clip_logg(3, "got xterm mouse event: type=%d y=%d x=%d buttons=%d clicks=%d", t, xterm_y, xterm_x, bb, clicks);
	 key = MOUSE_MASK;
	 key |= MOUSE_SET_TYPE(t);
	 key |= MOUSE_SET_ROW(xterm_y);
	 key |= MOUSE_SET_COL(xterm_x);
	 key |= MOUSE_SET_BUTTONS(bb);

	 dp->next_key = key;
	 *keyp = key;
	 return 1;
      }
   }

   if (dp->matchpos == 0)
   {
      /* if ( b>=lomatch && b<=himatch ) { */
      dp->matchno = -1;
      trymatch = 1;		/* check for begin of key string */
      /* } */
   }
   else
      trymatch = 1;

   has_esc = 0;

   if (trymatch)
   {
      if (dp->matchno < 0)
      {
	 Keytab *kp;

	 if (b == 27)
	    has_esc = 1;

	 kp = (Keytab *) bsearch(&b, keytab, key_count, sizeof(Keytab), cmp_first);

	 if (!kp)
	    goto end_match;
	 while (kp > keytab && kp->str[0] == (char) b)
	    kp--;
	 dp->matchno = kp - keytab;
      }
      for (; dp->matchno < key_count; dp->matchno++)
      {
	 Keytab *k = &keytab[dp->matchno];

	 if (dp->matchpos == k->strlen)
	    continue;
	 if (dp->matchpos > k->strlen)
	 {
	    dp->matchpos = 0;
	    return 0;
	 }
	 kb = k->str[dp->matchpos];
	 if (kb != b)
	    continue;

	 dp->matchpos++;
	 if (dp->matchpos == k->strlen)
	 {			/* match ended */
	    key = k->val;
	    dp->matchpos = 0;
	    /* dirty hack */
	    if (xterm_mouse && k->str && k->strlen == 3 && !strcmp(k->str, "\033[M"))
	    {
	       xterm_pos = 1;
	       return 0;
	    }
	    goto end_match;
	 }
	 return 0;		/* wait to continue matching */
      }

      dp->matchpos = 0;
      return 0;
   }

 end_match:
   if (!dp->has_meta2)
   {
      if (key == Meta1_key)
      {
	 dp->has_meta1 = 1;
	 return 0;
      }
      else if (key == Meta2_key)
      {
	 dp->has_meta2 = 1;
	 return 0;
      }
   }

   if (dp->national_mode && key >= 64 && key < 128)
      key = dp->nationalTable[key];
   else if (key >= 0 && key <= 255)
      key = dp->inputTable[key];
   dp->prev_has_meta1 = dp->has_meta1;
   dp->prev_has_meta2 = dp->has_meta2;
   if (dp->has_meta1)
   {
      key = META1(key);
      dp->has_meta1 = 0;
   }
   if (dp->has_meta2)
   {
      key = META2(key);
      dp->has_meta2 = 0;
   }

   if (key == National_key)
   {
      dp->national_mode = !dp->national_mode;
      return 0;
   }

   if (key == KEY_MOUSE)
   {
      if (xterm_mouse)
      {
	 xterm_pos = 1;
	 return 0;
      }
      else
	 key = KEY_F1;
   }

   dp->next_key = key;
   *keyp = key;

   return 1;
}
