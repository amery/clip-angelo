
CLIP_DLLEXPORT int
_clip_macroassign(ClipMachine * ClipMachineMemory, int op, int isTop, int fieldprec)
{
   int r;

   ClipVar *rval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 2;

   ClipVar *vp = _clip_vptr(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1);

   ClipVar *lval = 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "macro assign with non-character argument (%s)", _clip_typename(vp));
      return _clip_call_errblock(ClipMachineMemory, 1);
   }

   if (op == '=')
      r = _clip_nameassign(ClipMachineMemory, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			   vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, rval, fieldprec);
   else
   {
      lval = find_lval(ClipMachineMemory, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);

      if (!lval)
      {
	 _clip_trap_printf(ClipMachineMemory, __file__, __LINE__,
			   "no variable name: '%.*s'", vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
	 return _clip_call_errblock(ClipMachineMemory, 1);
      }

      if (op == '=')
	 r = _clip_mclone(ClipMachineMemory, lval, rval);
      else
	 r = do_assign(ClipMachineMemory, lval, rval, op);
   }

   if (r)
      return r;

   _clip_destroy(ClipMachineMemory, --(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame));

   if (isTop)
      _clip_destroy(ClipMachineMemory, --(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame));

   return 0;
}
