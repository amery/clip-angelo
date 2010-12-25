/*
   Opens a new window
 */
int
clip_WOPEN(ClipMachine * ClipMachineMemory)
{
   int top, left, bottom, right;

   int erase;

   int no = -1, i;

   ClipWindow *wp = 0;

   ClipRect wb = ClipMachineMemory->wboard;

   _clip_fullscreen(ClipMachineMemory);
   if (ClipMachineMemory->argc < 4)
      return EG_ARG;

   top = _clip_parni(ClipMachineMemory, 1);
   left = _clip_parni(ClipMachineMemory, 2);
   bottom = _clip_parni(ClipMachineMemory, 3);
   right = _clip_parni(ClipMachineMemory, 4);
   erase = _clip_parl(ClipMachineMemory, 5);

   {
      top = top > wb.top_of_ClipRect ? top : wb.top_of_ClipRect;
      left = left > wb.left_of_ClipRect ? left : wb.left_of_ClipRect;
      bottom = bottom < wb.bottom_of_ClipRect ? bottom : wb.bottom_of_ClipRect;
      right = right < wb.right_of_ClipRect ? right : wb.right_of_ClipRect;
   }

   if (top > bottom || left > right)
      return EG_ARG;

   save_window(ClipMachineMemory, ClipMachineMemory->windows + ClipMachineMemory->wnum);
   ClipMachineMemory->wnum++;
   ClipMachineMemory->windows = (ClipWindow *) realloc(ClipMachineMemory->windows, (ClipMachineMemory->wnum + 1) * sizeof(ClipWindow));
   wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   memset(wp, 0, sizeof(ClipWindow));
   for (i = 0, no = 0; i < ClipMachineMemory->wnum; i++)
      if (ClipMachineMemory->windows[i].no_of_ClipWindow > no)
	 no = ClipMachineMemory->windows[i].no_of_ClipWindow;
   wp->no_of_ClipWindow = no + 1;
   wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect = top;
   wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect = bottom;
   wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect = left;
   wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect = right;
   wp->ClipRect_format_of_ClipWindow = wp->ClipRect_rect_of_ClipWindow;
   wp->shadow_of_ClipWindow = ClipMachineMemory->wshadow;

   //if (ClipMachineMemory->wshadow)
   //      erase = 1;

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
