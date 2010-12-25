CLIP_DLLEXPORT double
_clip_parnd(ClipMachine * ClipMachineMemory, int num)
{
   if (num > 0 && num <= ClipMachineMemory->argc)
   {
      ClipVar *vp = _clip_vptr((ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1))));

      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && !vp->ClipType_t_of_ClipVar.memo_of_ClipType)
      {
	 return vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      }
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType && vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	 return rational_toDouble(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
   }
   return 0;
}
