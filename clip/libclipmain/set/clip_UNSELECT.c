int
clip_UNSELECT(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->colorSelect = 4;
   _clip_retc(ClipMachineMemory, "");
   return 0;
}
