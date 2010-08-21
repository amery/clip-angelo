/*
   Determines the usable area within a window
 */
int
clip_WFORMAT(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int dt, dl, db, dr;

   ClipRect format = wp->ClipRect_format_of_ClipWindow;

   _clip_fullscreen(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);
   if (ClipMachineMemory->argc < 4)
   {
      wp->ClipRect_format_of_ClipWindow = wp->ClipRect_rect_of_ClipWindow;
      return 0;
   }

   dt = _clip_parni(ClipMachineMemory, 1);

   dl = _clip_parni(ClipMachineMemory, 2);
   db = _clip_parni(ClipMachineMemory, 3);
   dr = _clip_parni(ClipMachineMemory, 4);

   format.top_of_ClipRect += dt;
   format.left_of_ClipRect += dl;
   format.bottom_of_ClipRect -= db;
   format.right_of_ClipRect -= dr;

   if (format.bottom_of_ClipRect - format.top_of_ClipRect < 0 || format.right_of_ClipRect - format.left_of_ClipRect < 0)
   {
      return 0;
   }

   if (format.top_of_ClipRect < wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect)
      format.top_of_ClipRect = wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;
   if (format.left_of_ClipRect < wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect)
      format.left_of_ClipRect = wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   if (format.bottom_of_ClipRect > wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect)
      format.bottom_of_ClipRect = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect;
   if (format.right_of_ClipRect > wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect)
      format.right_of_ClipRect = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect;

   wp->ClipRect_format_of_ClipWindow = format;

   adjust_cursor(ClipMachineMemory);

   return 0;
}
