CLIP_DLLEXPORT long
_clip_pardj(ClipMachine * ClipMachineMemory, int num)
{
   if (num > 0 && num <= ClipMachineMemory->argc)
   {
      ClipVar *vp = (ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1)));

      enum ClipVarType type = _clip_type(vp);

      if (type == DATE_type_of_ClipVarType)
      {
	 if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	    vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 return vp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
      }
   }
   return 0;
}
