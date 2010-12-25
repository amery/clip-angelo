CLIP_DLLEXPORT int
_clip_tcond(ClipMachine * ClipMachineMemory, int *ifp)
{
   ClipVar *vp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
   {
      _clip_trap_str(ClipMachineMemory, __file__, __LINE__, "non-logical var in condition");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
   if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
      *ifp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
   else
      *ifp = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;

   return 0;
}
