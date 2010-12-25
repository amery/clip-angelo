CLIP_DLLEXPORT int
_clip_not(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
   {
      _clip_trap_str(ClipMachineMemory, __file__, __LINE__, "non-logical type in NOT operator");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
   vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = !vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
   return 0;
}
