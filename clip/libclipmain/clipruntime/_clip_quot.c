CLIP_DLLEXPORT int
_clip_quot(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   ClipVar *rp = _clip_vptr(vp);

   char *s;

   int l;

   if (rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      l = rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      s = (char *) calloc(1, l + 3);
      memcpy(s + 1, rp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);
      s[0] = '"';
      s[l + 1] = '"';
      choose_quot(s, l + 2, s, s + l + 1);
      s[l + 2] = 0;
      l += 2;
   }
   else if (rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
   {
      int dd, mm, yy, ww;

      _clip_cdate(rp->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar, &dd, &mm, &yy, &ww);
      s = malloc(17);
      snprintf(s, 17, "STOD(\"%04d%02d%02d\")", yy, mm, dd);
      l = 16;
   }
   else if (rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATETIME_type_of_ClipVarType)
   {
      s = _clip_ttoc(ClipMachineMemory, rp->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar,
		     rp->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar, &l, ClipMachineMemory->date_format, ClipMachineMemory->hours, ClipMachineMemory->seconds);
   }
   else if (rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
   {
      s = malloc(4);
      snprintf(s, 4, "%s", rp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar ? ".T." : ".F.");
      l = 3;
   }
   else
      return 0;

   _clip_destroy(ClipMachineMemory, vp);

   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = s;
   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = l;
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;

   return 0;
}
