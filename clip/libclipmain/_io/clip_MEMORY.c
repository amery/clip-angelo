int
clip_MEMORY(ClipMachine * ClipMachineMemory)
{
   _clip_retnl(ClipMachineMemory, 0x7fffffffL);
   return 0;
}
