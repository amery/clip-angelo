/*
   Returns the position of the top row of the active window
 */
int
clip_WROW(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int lCenter = _clip_parl(ClipMachineMemory, 1);

   int r = wp->ClipRect_rect_of_ClipWindow.top_of_ClipRect;

   _clip_fullscreen(ClipMachineMemory);

   if (lCenter)
      calc_center(ClipMachineMemory, wp, &r, 0, 0, 0);

   _clip_retni(ClipMachineMemory, r);
   return 0;
}
