/*
   Determines the highest window handle
 */
int
clip_WNUM(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);
   _clip_retni(ClipMachineMemory, ClipMachineMemory->wnum);
   return 0;
}
