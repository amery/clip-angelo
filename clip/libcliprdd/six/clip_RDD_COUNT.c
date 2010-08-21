int
clip_RDD_COUNT(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, *ClipMachineMemory->ndbdrivers);
   return 0;
}
