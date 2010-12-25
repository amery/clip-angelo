int
_clip_restore_tty(ClipMachine * ClipMachineMemory)
{
   return restore_tty(ClipMachineMemory->screen->base);
}
