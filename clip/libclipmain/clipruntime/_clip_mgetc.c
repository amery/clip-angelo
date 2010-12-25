CLIP_DLLEXPORT int
_clip_mgetc(ClipMachine * ClipMachineMemory, ClipVar * vp, long no, char **strp, int *lenp)
{
   int ind;

   if (!_clip_mind(ClipMachineMemory, vp, no, &ind))
   {
      ClipVar *ep, *ap;

      int r;

      ap = _clip_vptr(vp);

      ep = &(ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl);
      r = _clip_strFromVar(ClipMachineMemory, ep, strp, lenp);
      return r;
   }
   return -1;
}
