CLIP_DLLEXPORT void
_clip_var_str(const char *str, int len, ClipVar * dest)
{
   dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (char *) _clip_memdup(str, len);
   dest->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
   dest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   dest->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   dest->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
}
