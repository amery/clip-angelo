CLIP_DLLEXPORT long
_clip_pardtj(ClipMachine * ClipMachineMemory, int num, long *time)
{
   if (num > 0 && num <= ClipMachineMemory->argc)
   {
      ClipVar *vp = (ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1)));

      enum ClipVarType type = _clip_type(vp);

      if (type == DATETIME_type_of_ClipVarType)
      {
	 if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	    vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 *time = vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar;
	 return vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar;
      }
   }
   return 0;
}
