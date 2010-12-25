/*
   Returns a window to the visible screen area, or centers it
 */
int
clip_WCENTER(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int lCenter = _clip_parl(ClipMachineMemory, 1);

   int top, left;

   _clip_fullscreen(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);

   calc_center(ClipMachineMemory, wp, &top, &left, 0, 0);
   if (lCenter)
      move_window(ClipMachineMemory, wp, top, left);
   else
      move_window(ClipMachineMemory, wp, top, left);

   return 0;
}
