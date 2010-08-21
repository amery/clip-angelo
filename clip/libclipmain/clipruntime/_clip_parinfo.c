CLIP_DLLEXPORT int
_clip_parinfo(ClipMachine * ClipMachineMemory, int num)
{
   if (num == 0)
      return ClipMachineMemory->argc;
   if (num < 0 || num > ClipMachineMemory->argc)
      return 0;

   return _clip_type(ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1)));
}
