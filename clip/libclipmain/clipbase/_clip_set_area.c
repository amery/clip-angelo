CLIP_DLLEXPORT int
_clip_set_area(ClipMachine * ClipMachineMemory, long area)
{
   DBWorkArea *wa;

   const char nm[] = "_clip_set_area";

   int no;

   if (!(wa = get_area(ClipMachineMemory, area, 0, &no)) && area)
      return rdd_err(ClipMachineMemory, EG_BADALIAS, 0, __FILE__, __LINE__, nm, er_badalias);

   ClipMachineMemory->curArea = no;
   return 0;
}
