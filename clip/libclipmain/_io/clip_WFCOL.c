/*
   Returns position of leftmost column of formatted area of window
 */
int
clip_WFCOL(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int lRelative = _clip_parl(ClipMachineMemory, 1);

   int r;

   _clip_fullscreen(ClipMachineMemory);

   if (lRelative)
      r = wp->ClipRect_format_of_ClipWindow.left_of_ClipRect - wp->ClipRect_rect_of_ClipWindow.left_of_ClipRect;
   else
      r = wp->ClipRect_format_of_ClipWindow.left_of_ClipRect;

   _clip_retni(ClipMachineMemory, r);
   return 0;
}
