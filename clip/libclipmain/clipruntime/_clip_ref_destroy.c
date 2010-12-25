CLIP_DLLEXPORT ClipVar *
_clip_ref_destroy(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   /*if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MREF_ClipFlags) */
   _clip_destroy(ClipMachineMemory, vp);
   return vp;
}
