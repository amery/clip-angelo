CLIP_DLLEXPORT ClipVar *
_clip_spar(ClipMachine * ClipMachineMemory, int num)
{
   if (num > 0 && num <= ClipMachineMemory->argc)
   {
      ClipVar *vp = (ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1)));

      return vp;
   }
   return 0;
}
