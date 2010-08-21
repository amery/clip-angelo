CLIP_DLLEXPORT int
_clip_calc_hash2(ClipMachine * ClipMachineMemory, int rev)
{
   ClipVar *vp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1, *vvp;

   long hash = -1, hash2 = -1;

   vvp = _clip_vptr(vp);

   if (vvp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
   {
      if (vvp->ClipType_t_of_ClipVar.memo_of_ClipType)
	 hash = (long) rational_toDouble(vvp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      else
	 hash = (long) vvp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
   }
   else if (vvp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      char *s = vvp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;

      int l = vvp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

      char *e;

      e = strstr(s, "->");
      if (e)
      {
	 hash = _clip_casehashword(e + 2, strlen(e + 2));
	 hash2 = _clip_casehashword(s, e - s);
      }
      else
	 hash = _clip_casehashword(s, l);
   }

   _clip_destroy(ClipMachineMemory, vp);
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 10;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (rev ? hash2 : hash);

   vp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   ++(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 10;
   vp->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
   vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (rev ? hash : hash2);

   CLIP_CHECK_STACK;
   return 0;
}
