CLIP_DLLEXPORT int
_clip_macro(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   vp = _clip_vptr(vp);

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      int l = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;

      char *s = _clip_memdup(vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, l);

      ClipVar *vpp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

      int r;

      _clip_destroy(ClipMachineMemory, vpp);

      r = _clip_eval_macro(ClipMachineMemory, s, l, vpp);
      free(s);

      return r;
   }
   else
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "macro operation with non-character argument (%s)", _clip_typename(vp));
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
}
