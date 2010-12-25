static void
adjust_cursor(ClipMachine * ClipMachineMemory)
{
   int x, y;

   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   x = ClipMachineMemory->screen->x;
   y = ClipMachineMemory->screen->y;
   if (y < wp->ClipRect_format_of_ClipWindow.top_of_ClipRect)
      y = wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
   if (y > wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect)
      y = wp->ClipRect_format_of_ClipWindow.bottom_of_ClipRect;
   if (x < wp->ClipRect_format_of_ClipWindow.left_of_ClipRect)
      x = wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
   if (x > wp->ClipRect_format_of_ClipWindow.right_of_ClipRect)
      x = wp->ClipRect_format_of_ClipWindow.right_of_ClipRect;
   y -= wp->ClipRect_format_of_ClipWindow.top_of_ClipRect;
   x -= wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;
   clip_region(ClipMachineMemory, &y, &x, 0, 0, 0, -1);

   ClipMachineMemory->screen->y = y;
   ClipMachineMemory->screen->x = x;
}
