CLIP_DLLEXPORT int
_clip_par_assign_str(ClipMachine * ClipMachineMemory, int num, char *str, int len)
{
   return _clip_par_assign_str_m(ClipMachineMemory, num, _clip_memdup(str, len), len);
}
