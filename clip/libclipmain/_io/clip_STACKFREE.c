int
clip_STACKFREE(ClipMachine * ClipMachineMemory)
{
   _clip_retnl(ClipMachineMemory, 0xFFFF);
   return 0;
}
