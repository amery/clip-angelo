int
clip_M6_ORDCREATE(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
   return clip_ORDCREATE(ClipMachineMemory);
}
