CLIP_DLLEXPORT void
_clip_var_log(int val, ClipVar * vp)
{
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = val;
}
