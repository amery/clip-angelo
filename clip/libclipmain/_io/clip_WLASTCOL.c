/*
   Returns position of the rightmost column of the active window
 */
int
clip_WLASTCOL(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int lCenter = _clip_parl(ClipMachineMemory, 1);

   int r = wp->ClipRect_rect_of_ClipWindow.right_of_ClipRect;

   _clip_fullscreen(ClipMachineMemory);

   if (lCenter)
      calc_center(ClipMachineMemory, wp, 0, 0, 0, &r);

   _clip_retni(ClipMachineMemory, r);
   return 0;
}
