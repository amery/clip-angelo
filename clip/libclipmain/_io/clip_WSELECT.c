/*
   Activates one of the open windows
 */
int
clip_WSELECT(ClipMachine * ClipMachineMemory)
{
   int no;

   int n = _clip_parni(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);

   no = (ClipMachineMemory->windows + ClipMachineMemory->wnum)->no_of_ClipWindow;
#if 0
   if (ClipMachineMemory->argc > 0 && n != no)
      select_window(ClipMachineMemory, n);
   _clip_retni(ClipMachineMemory, no);
#else
   if (ClipMachineMemory->argc > 0 && n != no)
      select_window(ClipMachineMemory, n);
   _clip_retni(ClipMachineMemory, (ClipMachineMemory->windows + ClipMachineMemory->wnum)->no_of_ClipWindow);
#endif
   return 0;
}
