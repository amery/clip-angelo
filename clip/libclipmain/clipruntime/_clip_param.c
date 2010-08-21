
CLIP_DLLEXPORT void
_clip_param(ClipMachine * ClipMachineMemory, int no, int lno)
{
   ClipVar *param = _clip_ref_local(ClipMachineMemory, lno);

   _clip_destroy(ClipMachineMemory, param);

   if (!param)
      return;
   if (no < ClipMachineMemory->argc)
   {
      ClipVar *vp = ClipMachineMemory->bp - (ClipMachineMemory->argc - no);

      if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags)
      {
	 _clip_destroy(ClipMachineMemory, param);
	 *param = *vp;
	 vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType++;
      }
      else
	 _clip_mclone(ClipMachineMemory, param, vp);
      if (param->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags && param->ClipType_t_of_ClipVar.field_of_ClipType)
      {
	 param->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	 param->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;
      }
   }
   else
      _clip_destroy(ClipMachineMemory, param);
}
