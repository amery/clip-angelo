int
clip_M6_CREATEINDEX(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
   return clip_DBCREATEINDEX(ClipMachineMemory);
}
