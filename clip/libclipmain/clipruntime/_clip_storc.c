CLIP_DLLEXPORT int
_clip_storc(ClipMachine * ClipMachineMemory, const char *str, int num, int ind)
{
   return _clip_storclen(ClipMachineMemory, str, strlen(str), num, ind);
}
