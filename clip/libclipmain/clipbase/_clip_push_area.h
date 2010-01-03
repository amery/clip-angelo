CLIP_DLLEXPORT int
_clip_push_area(ClipMachine * ClipMachineMemory, long area)
{
   DBWorkArea *wa;

   const char nm[] = "_clip_push_area";

   int       no;


   add_ClipVect((ClipMachineMemory->areaStack), (void *) (ClipMachineMemory->curArea));

   if (area > 0 && area <= 256)
    {
       ClipMachineMemory->curArea = area - 1;
       if (area > ClipMachineMemory->areas->count_of_ClipVect)
	{
	   area -= ClipMachineMemory->areas->count_of_ClipVect;
	   while (area--)
	      add_ClipVect(ClipMachineMemory->areas, NULL);
	}
    }
   else
    {
       if (!(wa = get_area(ClipMachineMemory, area, 0, &no)) && area)
	  return rdd_err(ClipMachineMemory, EG_BADALIAS, 0, __FILE__, __LINE__, nm, er_badalias);
       ClipMachineMemory->curArea = no;
    }

   return 0;
}
