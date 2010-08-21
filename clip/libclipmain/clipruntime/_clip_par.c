CLIP_DLLEXPORT ClipVar *
_clip_par(ClipMachine * ClipMachineMemory, int num)
{
   if (num > 0 && num <= ClipMachineMemory->argc)
   {
      ClipVar *vp = (ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1)));

      return _clip_vptr(vp);
   }
   return 0;
}
