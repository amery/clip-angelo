CLIP_DLLEXPORT void
_clear_trap(ClipMachine * ClipMachineMemory)
{
   int i;

   for (i = 0; i < ClipMachineMemory->trapTrace.count_of_ClipVect; ++i)
      free(ClipMachineMemory->trapTrace.items_of_ClipVect[i]);
   clear_ClipVect(&ClipMachineMemory->trapTrace);
}
