CLIP_DLLEXPORT int
_clip_strFromVar(ClipMachine * ClipMachineMemory, ClipVar * vp, char **str, int *lenp)
{
   if (!vp)
      return 0;

   vp = _clip_vptr(vp);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      {
	 ClipBuf buf = { 0, 0 };

	 int ret = _clip_expand(ClipMachineMemory, &buf, &vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar);

	 if (!ret)
	 {
	    *str = buf.buf_of_ClipBuf;
	    *lenp = buf.len_of_ClipBuf;
	 }
	 else
	 {
	    free(buf.buf_of_ClipBuf);
	    *str = 0;
	    *lenp = 0;
	 }
	 return ret;
      }
   default:
      *lenp = 0;
   }
   return 0;
}
