int
clip_NUMNOT(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, ~_clip_parni(ClipMachineMemory, 1));
   return 0;
}
