CLIP_DLLEXPORT void
_clip_retndp(ClipMachine * ClipMachineMemory, double n, int len, int dec)
{
   ClipVar *vp = ClipMachineMemory->bp - ClipMachineMemory->argc - 1;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = len;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = dec;
   if (!finite(n))
      n = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = n;
}
