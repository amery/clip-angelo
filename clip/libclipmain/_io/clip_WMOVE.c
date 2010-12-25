/*
   Moves a window
 */
int
clip_WMOVE(ClipMachine * ClipMachineMemory)
{
   ClipWindow *wp = ClipMachineMemory->windows + ClipMachineMemory->wnum;

   int top = _clip_parni(ClipMachineMemory, 1);

   int left = _clip_parni(ClipMachineMemory, 2);

   _clip_fullscreen(ClipMachineMemory);

   _clip_retni(ClipMachineMemory, wp->no_of_ClipWindow);

   move_window(ClipMachineMemory, wp, top, left);

   return 0;
}
