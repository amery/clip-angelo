CLIP_DLLEXPORT int
_clip_parl(ClipMachine * ClipMachineMemory, int num)
{
   if (num > 0 && num <= ClipMachineMemory->argc)
   {
      ClipVar *vp = (ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1)));

      enum ClipVarType type = _clip_type(vp);

      if (type == LOGICAL_type_of_ClipVarType)
      {
	 if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	    vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 return vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      }
   }
   return 0;
}
