CLIP_DLLEXPORT int
_clip_pop_area(ClipMachine * ClipMachineMemory)
{
   int no = ClipMachineMemory->areaStack->count_of_ClipVect;

   if (no < 1)
      return rdd_err(ClipMachineMemory, EG_NOALIAS, 0, __FILE__, __LINE__, "_clip_pop_area", er_noalias);

   ClipMachineMemory->areaStack->count_of_ClipVect--;
   ClipMachineMemory->curArea = (long) ClipMachineMemory->areaStack->items_of_ClipVect[no - 1];
   return 0;
}
