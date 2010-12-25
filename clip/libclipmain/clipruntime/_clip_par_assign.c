CLIP_DLLEXPORT int
_clip_par_assign(ClipMachine * ClipMachineMemory, int num, ClipVar * vp)
{
   ClipVar *lp = _clip_spar(ClipMachineMemory, num);

   if (!lp || !vp)
      return -1;
   if (!(lp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags))
      return 1;
   return _clip_mclone(ClipMachineMemory, lp, vp);
}
