CLIP_DLLEXPORT ClipVar *
_clip_mget(ClipMachine * ClipMachineMemory, ClipVar * vp, long no)
{
   int ind;

   if (!_clip_mind(ClipMachineMemory, vp, no, &ind))
   {
      ClipVar *ep, *ap;

      ap = _clip_vptr(vp);
      ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
      return ep;
   }

   return 0;
}
