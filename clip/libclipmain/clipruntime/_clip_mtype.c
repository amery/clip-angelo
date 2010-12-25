CLIP_DLLEXPORT int
_clip_mtype(ClipMachine * ClipMachineMemory, ClipVar * vp, long no)
{
   int ind;

   if (!_clip_mind(ClipMachineMemory, vp, no, &ind))
   {
      ClipVar *ap;

      ap = _clip_vptr(vp);
      return ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
   }

   return UNDEF_type_of_ClipVarType;
}
