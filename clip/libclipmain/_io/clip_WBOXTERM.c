/*
   Places a frame around the active window
 */
int
clip_WBOXTERM(ClipMachine * ClipMachineMemory)
{
   int cl = 0;

   unsigned char *chars = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &cl);

   unsigned char bchars[16];

   int chars_n = _clip_parni(ClipMachineMemory, 1);

   ClipRect rect, format;

   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int fill = 1;

   _clip_fullscreen(ClipMachineMemory);

   if (ClipMachineMemory->argc < 1)
      chars_n = 4;

   ClipMachineMemory->colorSelect = 0;

   rect = wp->ClipRect_rect_of_ClipWindow;
   format = wp->ClipRect_format_of_ClipWindow;

   format.top_of_ClipRect++;
   format.left_of_ClipRect++;
   format.bottom_of_ClipRect--;
   format.right_of_ClipRect--;

   if (format.bottom_of_ClipRect - format.top_of_ClipRect < 0 || format.right_of_ClipRect - format.left_of_ClipRect < 1)
   {
      _clip_retni(ClipMachineMemory, -1);
      return 0;
   }

   if (chars)
   {
      cl = strlen((const char *) chars);
      if (cl >= sizeof(bchars))
	 cl = sizeof(bchars) - 1;

      memcpy(bchars, chars, cl);
      if (cl < 9)
	 bchars[8] = *((char *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb));
      chars = bchars;
   }
   disp_box(ClipMachineMemory, 0, 0,
	    wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect - wp->ClipRect_format_of_ClipWindow.top_of_ClipRect,
	    wp->ClipRect_format_of_ClipWindow.right_of_ClipRect - wp->ClipRect_format_of_ClipWindow.left_of_ClipRect, chars, cl, chars_n, 0, fill);

   wp->ClipRect_format_of_ClipWindow = format;
   if (fill)
   {
      ClipMachineMemory->screen->x = 0;
      ClipMachineMemory->screen->y = 0;
   }
   adjust_cursor(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, ClipMachineMemory->wnum);
   return 0;
}
