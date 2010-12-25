CLIP_DLLEXPORT int
_clip_refmacro(ClipMachine * ClipMachineMemory)
{
   VarEntry *vep;

   long fhash = 0, ahash;

   char *fname = 0, *aname = 0;

   int fl = 0, al = 0, r;

   ClipVar *lval = 0;

   ClipVar *sp = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   ClipVar *vp = _clip_vptr(sp);

   long *dim = 0;

   int ndim = 0;

   if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__, "macro reference with non-character argument (%s)", _clip_typename(vp));
      return 0;
   }

   r = _clip_parse_name(ClipMachineMemory, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
			vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, &aname, &al, &fname, &fl, &dim, &ndim);

   if (aname)
      ahash = _clip_casehashbytes(0, aname, al);
   else
      ahash = -1;

   if (fname)
      fhash = _clip_casehashbytes(0, fname, fl);

   _clip_destroy(ClipMachineMemory, sp);
   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame--;

   if (r == 2)			/* field */
   {
      return _clip_field(ClipMachineMemory, fhash, ahash);
   }
   else if (r == 1)		/* memvar */
   {
   }
   else
      /* field/memvar */
   {
      if (!_clip_try_field(ClipMachineMemory, fhash))
	 return 0;
   }

   vep = fetch_var(ClipMachineMemory, fhash);

   if (vep)
      lval = &vep->ClipVar_var_of_VarEntry;

   if (!lval)
   {
      _clip_trap_printf(ClipMachineMemory, __file__, __LINE__,
			"no variable name: '%.*s'", vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
      return _clip_call_errblock(ClipMachineMemory, 1);
   }
   else if (r == 3)
   {
      lval = _clip_aref(ClipMachineMemory, lval, ndim, dim);
      free(dim);
   }

   _clip_ref(ClipMachineMemory, lval, 0);

   return 0;
}
