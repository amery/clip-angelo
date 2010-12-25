CLIP_DLLEXPORT int
_clip_par_assign_str_m(ClipMachine * ClipMachineMemory, int num, char *str, int len)
{
   ClipVar v;

   int r;

   if (!_clip_par_isref(ClipMachineMemory, num))
      return -1;
   CLEAR_CLIPVAR(&v);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = str;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   r = _clip_par_assign(ClipMachineMemory, num, &v);
   _clip_destroy(ClipMachineMemory, &v);
   return r;
}
