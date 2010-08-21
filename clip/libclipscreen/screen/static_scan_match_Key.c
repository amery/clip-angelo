static int
scan_match_Key(ScreenBase * base, unsigned char b, unsigned long *keyp)
{
   long key;

   if (xterm_mouse && b == RAWMODE_ESC)
   {
      xterm_pos = 1;
      return 0;
   }

   if (xterm_mouse && xterm_pos)
   {
      int mb, t, bb = 0;
      static struct timeval tv1 = { 0, 0 }, tv2;
      static int clicks = 0;

      switch (xterm_pos)
      {
      case 1:
	 xterm_buttons = b;
	 xterm_pos = 2;
	 return 0;
      case 2:
	 xterm_x = b;
	 xterm_pos = 3;
	 return 0;
      case 3:
      default:
	 xterm_y = b;
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

	 if (xterm_buttons & 0x4)
	 {
	    t = MOUSE_TYPE_MOVE;
	    bb = base->mouse_buttons;
	 }
	 else
	 {
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

	       t = MOUSE_TYPE_DOWN;
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
	 }

	 _clip_logg(3, "got xterm rawmode mouse event: type=%d y=%d x=%d xt_buttons=%d buttons=%d clicks=%d", t, xterm_y, xterm_x, xterm_buttons, bb, clicks);
	 key = MOUSE_MASK;
	 key |= MOUSE_SET_TYPE(t);
	 key |= MOUSE_SET_ROW(xterm_y);
	 key |= MOUSE_SET_COL(xterm_x);
	 key |= MOUSE_SET_BUTTONS(bb);

	 *keyp = key;
	 return 1;
      }
   }

   key = scan_key(b);

   if (key)
   {
      *keyp = key;
      //scan_reset();
      return 1;
   }
   else
      return 0;
}
