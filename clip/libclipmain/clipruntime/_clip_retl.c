CLIP_DLLEXPORT void
_clip_retl(ClipMachine * ClipMachineMemory, int n)
{
   ClipVar *vp = ClipMachineMemory->bp - ClipMachineMemory->argc - 1;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = n;
}
