CLIP_DLLEXPORT ClipVar *
_clip_vptr(ClipVar * vp)
{
   if (vp && vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
      vp = _clip_vptr(vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
   return vp;
}
