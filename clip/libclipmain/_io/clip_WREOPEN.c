int
clip_WREOPEN(ClipMachine * ClipMachineMemory)
{
   int top, left, bottom, right;

   int erase, i;

   ClipWindow *wp = 0;

   _clip_fullscreen(ClipMachineMemory);
   if (ClipMachineMemory->argc < 4)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 1);
   left = _clip_parni(ClipMachineMemory, 2);
   bottom = _clip_parni(ClipMachineMemory, 3);
   right = _clip_parni(ClipMachineMemory, 4);
   erase = _clip_parl(ClipMachineMemory, 5);

   if (top > bottom || left > right)
      return EG_ARG;

   //save_window(ClipMachineMemory, ClipMachineMemory->windows + ClipMachineMemory->wnum);
   wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect = top;
   wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect = bottom;
   wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect = left;
   wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect = right;
   wp->ClipRect_format_of_ClipWindow = wp->ClipRect_rect_of_ClipWindow;

   ClipMachineMemory->screen->y = 0;
   ClipMachineMemory->screen->x = 0;
   adjust_cursor(ClipMachineMemory);

   if (erase)
   {
      int attr = _clip_colorSelect(ClipMachineMemory);

      int j;

      char fill = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb));

      Screen *sp = ClipMachineMemory->screen;

      bottom -= top;
      top = 0;
      right -= left;
      left = 0;
      clip_region(ClipMachineMemory, &top, &left, &bottom, &right, 2, -1);
      for (i = top; i <= bottom; ++i)
      {
	 sp->touched[i] = 1;
	 for (j = left; j <= right; ++j)
	 {
	    sp->chars[i][j] = fill;
	    sp->colors[i][j] = attr;
	    sp->attrs[i][j] = 0;
	 }
      }
      draw_shadow(ClipMachineMemory, wp);
      sync_mp(ClipMachineMemory);
   }

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);

   return 0;
}
