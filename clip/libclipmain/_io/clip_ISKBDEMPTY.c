int
clip_ISKBDEMPTY(ClipMachine * ClipMachineMemory)
{
   _clip_fullscreen(ClipMachineMemory);
   _clip_retl(ClipMachineMemory, *ClipMachineMemory->kbdptr == ClipMachineMemory->kbdbuf);
   return 0;
}
