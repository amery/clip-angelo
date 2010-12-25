int
clip_M6_DBLOCATE(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
   return clip___DBLOCATEOPTIMIZE(ClipMachineMemory);
}
