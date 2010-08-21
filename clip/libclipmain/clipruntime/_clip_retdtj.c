CLIP_DLLEXPORT void
_clip_retdtj(ClipMachine * ClipMachineMemory, long julian, long time)
{
   ClipVar *vp = RETPTR(ClipMachineMemory);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf != NULL)
      free(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATETIME_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 8;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar = julian;
   vp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar = time;
}
