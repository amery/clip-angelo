/*
   Returns the position of the bottom row of the active window
 */
int
clip_WLASTROW(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int lCenter = _clip_parl(ClipMachineMemory, 1);

   int r = wp->ClipRect_rect_of_ClipWindow.bottom_of_ClipRect;

   _clip_fullscreen(ClipMachineMemory);

   if (lCenter)
      calc_center(ClipMachineMemory, wp, 0, 0, &r, 0);

   _clip_retni(ClipMachineMemory, r);
   return 0;
}
