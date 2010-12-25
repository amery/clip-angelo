CLIP_DLLEXPORT long
_clip_pardc(ClipMachine * ClipMachineMemory, int num, int *yy, int *mm, int *dd, int *ww)
{
   long julian = _clip_pardj(ClipMachineMemory, num);

   _clip_cdate(julian, dd, mm, yy, ww);
   return julian;
}
