CLIP_DLLEXPORT int
_clip_func_name(struct ClipMachine *ClipMachineMemory, int argc)
{
   ClipFrame *fp = ClipMachineMemory->fp;

   ClipVar *vp = _clip_vptr(fp->ClipVar_sp_of_ClipFrame - argc - 2);

   long hash;

   int ret;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
      hash = _clip_casehashbytes(0, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
   else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
   {
      if (vp->ClipType_t_of_ClipVar.memo_of_ClipType)
	 hash = (long) rational_toDouble(vp->ClipRationalVar_r_of_ClipVar.Struc_rational_of_ClipRationalVar);
      else
	 hash = (long) vp->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
   }
   else
   {
      _clip_trap_str(ClipMachineMemory, __file__, __LINE__, "call by name not from CHARACTER_type_of_ClipVarType or NUMERIC_type_of_ClipVarType value");
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
   if ((ret = _clip_func_hash(ClipMachineMemory, hash, argc, 0, 0)))
      return ret;
   --fp->ClipVar_sp_of_ClipFrame;
   _clip_destroy(ClipMachineMemory, fp->ClipVar_sp_of_ClipFrame - 1);
   fp->ClipVar_sp_of_ClipFrame[-1] = fp->ClipVar_sp_of_ClipFrame[0];

   return 0;
}
