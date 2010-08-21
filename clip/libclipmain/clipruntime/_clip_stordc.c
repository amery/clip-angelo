CLIP_DLLEXPORT int
_clip_stordc(ClipMachine * ClipMachineMemory, int yy, int mm, int dd, int num, int ind)
{
   return _clip_stordj(ClipMachineMemory, _clip_jdate(dd, mm, yy), num, ind);
}
