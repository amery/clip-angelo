int
clip_ENHANCED(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->colorSelect = 1;
   _clip_retc(ClipMachineMemory, "");
   return 0;
}
