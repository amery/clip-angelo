CLIP_DLLEXPORT int
_clip_func_hash(struct ClipMachine *ClipMachineMemory, long hash, int argc, int rest, ClipVarFrame * uplocals)
{
   ClipFunction *fp;

   ClipBlock *bp;

   int r = 0;

   ClipFrame *inMacro = ClipMachineMemory->inMacro;

   ClipMachineMemory->inMacro = 0;
   if (!_clip_get_function(ClipMachineMemory, hash, &fp, &bp))
   {
      char buf[64];

      VarEntry *vep;

      vep = fetch_var(ClipMachineMemory, hash);

      if (vep
	  && (vep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType
	      || vep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType))
      {
	 /* support FoxPro syntax aVar(nElement) */
	 ClipVar *vp;

	 ClipVar *ap = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - argc - 1;

	 _clip_clone(ClipMachineMemory, ap, &vep->ClipVar_var_of_VarEntry);
	 vp = clip_fetch(ClipMachineMemory, argc, 1, 0, 0, 0);
	 if (vp)
	    goto ret;
      }
      if (vep
	  && (vep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType
	      || vep->ClipVar_var_of_VarEntry.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType))
      {
	 /* if function not found - call codeblock from memvar */
	 int _argc = ClipMachineMemory->argc;

	 ClipFrame *fp = ClipMachineMemory->fp;

	 ClipVar *bp = ClipMachineMemory->bp;

	 ClipVar res;

	 memset(&res, 0, sizeof(res));

	 ClipMachineMemory->argc = argc;
	 ClipMachineMemory->bp = fp->ClipVar_sp_of_ClipFrame;
	 r = _clip_eval(ClipMachineMemory, &(vep->ClipVar_var_of_VarEntry), ClipMachineMemory->argc, ARGPTR(ClipMachineMemory, 1), &res);
	 //r = _clip_eval(ClipMachineMemory, &(vep->ClipVar_var_of_VarEntry), ClipMachineMemory->argc,0, &res);
	 ClipMachineMemory->argc = _argc;
	 ClipMachineMemory->fp = fp;
	 ClipMachineMemory->bp = bp;
	 argc++;
	 for (; argc; --argc)
	 {
	    --(fp->ClipVar_sp_of_ClipFrame);
	    _clip_destroy(ClipMachineMemory, fp->ClipVar_sp_of_ClipFrame);
	 }
	 _clip_push(ClipMachineMemory, &res);
	 _clip_destroy(ClipMachineMemory, &res);
	 r = 0;
	 goto ret;
      }
#if 1
      {
	 /* call error with args */
	 int _argc = ClipMachineMemory->argc;

	 ClipFrame *fp = ClipMachineMemory->fp;

	 ClipVar *bp = ClipMachineMemory->bp;

	 ClipVar res;

	 memset(&res, 0, sizeof(res));

	 ClipMachineMemory->argc = argc;
	 ClipMachineMemory->bp = fp->ClipVar_sp_of_ClipFrame;
	 _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
	 r = _clip_trap_err(ClipMachineMemory, EG_NOFUNC, 0, 0, 0, 0, buf);
	 ClipMachineMemory->argc = _argc;
	 ClipMachineMemory->fp = fp;
	 ClipMachineMemory->bp = bp;
	 if (!ClipMachineMemory->trapVar)
	    _clip_generr(ClipMachineMemory, r);
	 _clip_errorblock_res(ClipMachineMemory, ClipMachineMemory->trapVar, r, &res);
	 argc++;
	 for (; argc; --argc)
	 {
	    --(fp->ClipVar_sp_of_ClipFrame);
	    _clip_destroy(ClipMachineMemory, fp->ClipVar_sp_of_ClipFrame);
	 }
	 _clip_push(ClipMachineMemory, &res);
	 _clip_destroy(ClipMachineMemory, &res);
	 r = 0;
	 goto ret;
      }
#else
      {
	 /* old code for call error without args */
	 _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
	 r = _clip_trap_err(ClipMachineMemory, EG_NOFUNC, 0, 0, 0, 0, buf);
	 r = _clip_call_errblock(ClipMachineMemory, r);
      }
#endif
   }
   if (fp)
   {
      char *oldbuf = 0, *buf = 0;

      if (log_level >= 4)
      {
	 if (ClipMachineMemory->fp->procname_of_ClipFrame)
	 {
	    buf = (char *) calloc(1, 64);
	    _clip_hash_name(ClipMachineMemory, hash, buf, 64);
	    oldbuf = (char *) ClipMachineMemory->fp->procname_of_ClipFrame;
	    ClipMachineMemory->fp->procname_of_ClipFrame = buf;
	 }
      }

      r = _clip_func(ClipMachineMemory, fp, argc, rest, uplocals);

      if (log_level >= 4)
      {
	 if (oldbuf)
	 {
	    ClipMachineMemory->fp->procname_of_ClipFrame = oldbuf;
	    free(buf);
	 }
      }

   }
   else
      r = _clip_code_func(ClipMachineMemory, bp, argc, rest, uplocals);

 ret:
   ClipMachineMemory->inMacro = inMacro;
   return r;
}
