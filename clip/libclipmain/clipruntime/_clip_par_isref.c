CLIP_DLLEXPORT int
_clip_par_isref(ClipMachine * ClipMachineMemory, int num)
{
   ClipVar *vp = _clip_spar(ClipMachineMemory, num);

   if (!vp)
      return 0;
   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
      return 1;
   return 0;
}
