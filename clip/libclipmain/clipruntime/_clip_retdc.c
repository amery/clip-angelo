CLIP_DLLEXPORT void
_clip_retdc(ClipMachine * ClipMachineMemory, int yy, int mm, int dd)
{
   _clip_retdj(ClipMachineMemory, _clip_jdate(dd, mm, yy));
}
