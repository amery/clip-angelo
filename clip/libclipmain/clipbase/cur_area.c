DBWorkArea *
cur_area(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = 0;

   if (!ClipMachineMemory->areas->count_of_ClipVect)
      return NULL;
   wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[ClipMachineMemory->curArea];
   if (wa && !wa->used)
      return NULL;
   return wa;
}
