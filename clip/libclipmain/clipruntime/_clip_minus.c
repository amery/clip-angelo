CLIP_DLLEXPORT int
_clip_minus(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
   {
      _clip_trap_str(ClipMachineMemory, __file__, __LINE__, "non-numeric type in MINUS operator");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
   if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
      rational_inverse(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
   else
      vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = -vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
   return 0;
}
