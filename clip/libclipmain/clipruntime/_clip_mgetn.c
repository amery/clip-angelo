CLIP_DLLEXPORT int
_clip_mgetn(ClipMachine * ClipMachineMemory, ClipVar * vp, long no, double *n)
{
   int ind;

   if (!_clip_mind(ClipMachineMemory, vp, no, &ind))
   {
      ClipVar *ep, *ap;

      ap = _clip_vptr(vp);

      ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
      *n = _clip_double(ep);
      return 0;
   }
   return -1;
}
