/*
   Returns position of the leftmost column of the active window
 */
int
clip_WCOL(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int lCenter = _clip_parl(ClipMachineMemory, 1);

   int r = wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;

   _clip_fullscreen(ClipMachineMemory);

   if (lCenter)
      calc_center(ClipMachineMemory, wp, 0, &r, 0, 0);

   _clip_retni(ClipMachineMemory, r);
   return 0;
}
