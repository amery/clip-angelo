CLIP_DLLEXPORT int
_clip_expand_var(ClipMachine * ClipMachineMemory, ClipVar * vp)
{
   ClipBuf dest;

   int ret;

   char *s, *e;

   vp = _clip_vptr(vp);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      ret = _clip_expand(ClipMachineMemory, &dest, &(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar));
      if (ret)
	 return ret;
   }
   else
   {
      _clip_destroy(ClipMachineMemory, vp);
      vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MSTAT_ClipFlags;
      vp->ClipType_t_of_ClipVar.len_of_ClipType = 0;
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = "";
      vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 0;
      return 0;
   }
   _clip_destroy(ClipMachineMemory, vp);
   vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = 0;
   vp->ClipType_t_of_ClipVar.len_of_ClipType = 1;
   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar = dest;
   for (s = dest.buf_of_ClipBuf, e = dest.buf_of_ClipBuf + dest.len_of_ClipBuf; s < e; ++s)
      if (*s == '&')
      {
	 ret = 1;
	 break;
      }
   if (!ret)
      vp->ClipType_t_of_ClipVar.len_of_ClipType = 0;
   return 0;
}
