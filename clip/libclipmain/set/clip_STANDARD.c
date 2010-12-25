int
clip_STANDARD(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->colorSelect = 0;
   _clip_retc(ClipMachineMemory, "");
   return 0;
}
