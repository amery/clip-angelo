int
clip_ISCOLOR(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);
   _clip_retl(ClipMachineMemory, iscolor_tty(ClipMachineMemory->screen->base));
   return 0;
}
