CLIP_DLLEXPORT int
_clip_mclone(struct ClipMachine *ClipMachineMemory, ClipVar * dest, ClipVar * src)
{
   if (dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags && dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType > 1)
   {
      ClipVar *dp, *srcp;

      int count, ret;

      dp = dest->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
      srcp = src;
      count = dp->ClipType_t_of_ClipVar.count_of_ClipType;
      ret = 0;

      switch (src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType)
      {
      case F_MREF_ClipFlags:
	 srcp = src->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if (srcp == dp)
	    break;
      case F_MPTR_ClipFlags:
	 if (src->ClipType_t_of_ClipVar.field_of_ClipType && src->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar)
	 {
	    dest->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = src->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar;
	    dest->ClipType_t_of_ClipVar.field_of_ClipType = 1;
	 }
      default:
	 _clip_destroy(ClipMachineMemory, dp);
	 ret = _clip_clone(ClipMachineMemory, dp, srcp);
	 dp->ClipType_t_of_ClipVar.count_of_ClipType = count;
	 dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = srcp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
	 dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
	 dest->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 break;
      }
      return ret;
   }
   else
   {
      _clip_destroy(ClipMachineMemory, dest);
      return _clip_clone(ClipMachineMemory, dest, src);
   }
}
