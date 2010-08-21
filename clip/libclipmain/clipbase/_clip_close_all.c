CLIP_DLLEXPORT void
_clip_close_all(ClipMachine * ClipMachineMemory)
{
   int i;

   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
      close_area(ClipMachineMemory, (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[i], "DBCLOSEALL");
}
