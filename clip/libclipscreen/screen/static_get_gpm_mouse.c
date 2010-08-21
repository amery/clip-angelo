static unsigned long
get_gpm_mouse(ScreenBase * base)
{
   unsigned long key = 0;

   int r;

   Gpm_Event event;

   r = gpm_get_event(&event);
   if (r > 0)
   {
      if (event.x < base->mouse_left)
	 event.x = base->mouse_left;
      if (event.x > base->mouse_right)
	 event.x = base->mouse_right;
      if (event.y < base->mouse_top)
	 event.y = base->mouse_top;
      if (event.y > base->mouse_bottom)
	 event.y = base->mouse_bottom;
      base->mouse_x = event.x;
      base->mouse_y = event.y;
      base->mouse_auto_visible = 1;
      if (event.type & GPM_DOWN)
	 base->mouse_buttons = event.buttons;
      else if (event.type & GPM_UP)
      {
	 if (event.buttons & GPM_B_LEFT)
	    base->mouse_clicks_left++;
	 if (event.buttons & GPM_B_RIGHT)
	    base->mouse_clicks_right++;
	 if (event.buttons & GPM_B_MIDDLE)
	    base->mouse_clicks_middle++;
	 base->mouse_buttons = 0;
      }
   }
   _clip_logg(3, "got gp_get_event: %d type=%d y=%d x=%d buttons=%d m_buttons=%d", r, event.type, event.y, event.x, event.buttons, base->mouse_buttons);
   if (r == 1)
   {
      key = MOUSE_MASK;
      key |= MOUSE_SET_TYPE(event.type);
      key |= MOUSE_SET_ROW(event.y);
      key |= MOUSE_SET_COL(event.x);
      key |= MOUSE_SET_BUTTONS(event.buttons);
   }
   return key;
}
