int
clip_M6_ERROR(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, ClipMachineMemory->m6_error);
   return 0;
}
