CLIP_DLLEXPORT int
_clip_mgetl(ClipMachine * ClipMachineMemory, ClipVar * vp, long no, int *n)
{
   int ind;

   if (!_clip_mind(ClipMachineMemory, vp, no, &ind))
   {
      ClipVar *ep, *ap;

      ap = _clip_vptr(vp);

      ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
      *n = _clip_log(ep);
      return 0;
   }
   return -1;
}
