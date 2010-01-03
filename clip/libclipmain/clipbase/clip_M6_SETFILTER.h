int
clip_M6_SETFILTER(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
   return clip_DBSETFILTER(ClipMachineMemory);
}
