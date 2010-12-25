CLIP_DLLEXPORT char *
_clip_parcl(ClipMachine * ClipMachineMemory, int num, int *len)
{
   if (num > 0 && num <= ClipMachineMemory->argc)
   {
      ClipVar *vp = (ClipMachineMemory->bp - (ClipMachineMemory->argc - (num - 1)));

      enum ClipVarType type = _clip_type(vp);

      if (type == CHARACTER_type_of_ClipVarType)
      {
	 if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
	    vp = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if (len)
	    *len = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 return vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf;
      }
   }
   return 0;
}
