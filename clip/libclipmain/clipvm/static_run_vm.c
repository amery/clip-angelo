static int
run_vm(ClipMachine * ClipMachineMemory, ClipBlock * bp)
{
   char *func = bp->function_of_ClipBlock;

   char *modbeg = func - GETLONG(func);

   char *pp = modbeg + GETLONG(F_OFFS(func, 1, 0, 0));

   char code;

   long len = GETLONG(F_OFFS(func, 2, 0, 0));

   char *beg = pp, *end = pp + len;

   long staticsOffs = GETLONG(modbeg);

   ClipFile *file = bp->file_of_ClipBlock;

   ClipVar *statics = file->ClipVar_statics_of_ClipFile + staticsOffs;

   int nlocals = GETSHORT(F_OFFS(func, 3, 0, 1));

   int nreflocals = GETSHORT(F_OFFS(func, 3, 1, 1));

   int ret = 0;

   int i;

#if 0
   ClipVar *locals /* = (ClipVar *) alloca(sizeof(ClipVar) * nlocals) */ ;
#endif
   int maxdeep = GETSHORT(F_OFFS(func, 3, 2, 1));

#ifdef OS_MINGW
   ClipVar *stack = (ClipVar *) malloc(sizeof(ClipVar) * maxdeep);
#else
   ClipVar *stack = (ClipVar *) alloca(sizeof(ClipVar) * maxdeep);
#endif
   char *filename = F_OFFS(modbeg, 7, 4, 0);

   int nprivates = GETSHORT(F_OFFS(func, 3, 3, 1));

   /*int nparams = GETSHORT( F_OFFS(func, 3, 4, 1)); */
   /*long *privates = (long *) F_OFFS(func, 3, 5, 1); */
   /*long *localDefHashs = (long *) F_OFFS(func, 3 + nprivates, 5, 1); */
   int numlocals = nlocals + nreflocals;

   /*ClipVarDef *ldp, *localDefs = (ClipVarDef *) alloca(sizeof(ClipVarDef) * (numlocals+1)); */
   /*short *localDefNums = (short *) F_OFFS(func, 3 + nprivates + numlocals, 5, 1); */

   char *procname = F_OFFS(func, 3 + nprivates + numlocals, 5 + numlocals, 1 + 1);

   char *localnames = procname + *(unsigned char *) F_OFFS(func, 3 + nprivates + numlocals,
							   5 + numlocals, 1) + 1;

   /*ClipVar *params = (ClipVar *)alloca( nparams*sizeof(ClipVar)); */
   ClipFrame frame = { stack, stack, filename, 0, 0, 0, 0 /*localDefs */ , file->ClipVarDef_staticDefs_of_ClipFile, 0,
      file->hash_names_of_ClipFile, procname, maxdeep, 0
   };
   unsigned char c, c1;

   short s, s1;

   long l, l1;

   ClipVar *vp;

   ClipBlock bl;

   char *trap_pp = 0;

   int debuglevel = _clip_debuglevel;

   ClipVarFrame *localvars = 0, *reflocals = 0;

   int local_locals = 0;

   if (!nlocals && !nreflocals)
   {
      reflocals = frame.ClipVarFrame_localvars_of_ClipFrame = ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame;
      if (reflocals && reflocals->refcount_of_ClipVarFrame)
	 reflocals->refcount_of_ClipVarFrame++;
   }
   else if (nreflocals)
   {
      localvars = (ClipVarFrame *) calloc(1, sizeof(ClipVarFrame) + numlocals * sizeof(ClipVar));
      localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
      localvars->refcount_of_ClipVarFrame = 1;
      reflocals = frame.ClipVarFrame_localvars_of_ClipFrame = localvars;
      localvars->size_of_ClipVarFrame = numlocals;
      localvars->names_of_ClipVarFrame = localnames;
   }
   else
   {
#if 1
      localvars = (ClipVarFrame *) calloc(1, sizeof(ClipVarFrame) + numlocals * sizeof(ClipVar));
      localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
      localvars->size_of_ClipVarFrame = numlocals;
      localvars->refcount_of_ClipVarFrame = 1;
      localvars->names_of_ClipVarFrame = localnames;
      reflocals = frame.ClipVarFrame_localvars_of_ClipFrame = localvars;
#else
      locals = (ClipVar *) alloca(sizeof(ClipVar) * numlocals);
      memset(locals, 0, numlocals * sizeof(ClipVar));
      localvars = (ClipVarFrame *) alloca(sizeof(ClipVarFrame));
      localvars->vars = locals;
      localvars->size_of_ClipVarFrame_of_ClipVarFrame = numlocals;
      localvars->refcount_of_ClipVarFrame = 0;
      localvars->names_of_ClipVarFrame = localnames;
      reflocals = frame.ClipVarFrame_localvars_of_ClipFrame = localvars;
      local_locals = 1;
#endif
   }

   frame.ClipFrame_up_of_ClipFrame = ClipMachineMemory->fp;
   ClipMachineMemory->fp = &frame;

#if 0
   localDefs[0].name = numlocals;
   for (i = 0, ldp = localDefs + 1; i < numlocals; i++, ldp++, localDefHashs++, localDefNums++)
   {
      /*int no = *localDefNums; */
      long hash = *localDefHashs;

      ldp->name = hash;
      /*
         if (no < 0)
         ldp->vp = reflocals - no;
         else
       */
      /*ldp->vp = locals + no; */
      ldp->vp = 0;
   }
#endif

   if (_clip_profiler)
      _clip_start_profiler(ClipMachineMemory);

   _clip_logg(4, "PCODE call: proc '%s' file '%s' line %d", frame.procname_of_ClipFrame ? frame.procname_of_ClipFrame : "unknown", frame.filename_of_ClipFrame, frame.line_of_ClipFrame);

 cont:
   while (pp >= beg && pp < end)
   {
      if (debuglevel)
	 _clip_debug(ClipMachineMemory);
      switch ((code = *pp++))
      {
      case CLIP_NOP:
	 break;
      case CLIP_POP:
	 _clip_pop(ClipMachineMemory);
	 break;
      case CLIP_LINE:
	 frame.line_of_ClipFrame = GET_SHORT(pp);
	 if (debuglevel)
	    _clip_line(ClipMachineMemory);
	 break;
      case CLIP_PUSH_NUM:
	 {
	    int len, dec;

	    double d;

	    s = GET_SHORT(pp);
	    frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	    frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	    get_num(modbeg, s, &d, &len, &dec);
	    frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.len_of_ClipType = len;
	    frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.dec_of_ClipType = dec;
	    frame.ClipVar_sp_of_ClipFrame->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = d;
	    frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	    ++frame.ClipVar_sp_of_ClipFrame;
	 }
	 break;
      case CLIP_PUSH_STR:
#if 0
	 s = GET_SHORT(pp);
	 frame.sp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 frame.sp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MSTAT_ClipFlags;
	 frame.sp->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 get_str(modbeg, s, &(frame.sp->s.ClipBuf_str_of_ClipStrVar.buf), &(frame.sp->s.ClipBuf_str_of_ClipStrVar.len));
	 /*
	    if ( !(ClipMachineMemory->flags1 & NOEXPAND_MACRO_FLAG) )
	    {
	  */
	 if (strchr(frame.sp->s.ClipBuf_str_of_ClipStrVar.buf, '&'))
	    _clip_expand_var(ClipMachineMemory, frame.sp);
	 /*} */
	 ++frame.sp;
	 break;
#endif
      case CLIP_PUSH_STRDUP:
	 s = GET_SHORT(pp);
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 {
	    char *str = "";

	    int len = 0;

	    get_str(modbeg, s, &str, &len);
	    frame.ClipVar_sp_of_ClipFrame->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = _clip_memdup(str, len);
	    frame.ClipVar_sp_of_ClipFrame->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = len;
	 }
	 /*
	    if ( !(ClipMachineMemory->flags1 & NOEXPAND_MACRO_FLAG) )
	    {
	  */
	 if (strchr(frame.ClipVar_sp_of_ClipFrame->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, '&'))
	    _clip_expand_var(ClipMachineMemory, frame.ClipVar_sp_of_ClipFrame);
	 /*} */
	 ++frame.ClipVar_sp_of_ClipFrame;
	 break;
      case CLIP_PUSH_NIL:
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = UNDEF_type_of_ClipVarType;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 ++frame.ClipVar_sp_of_ClipFrame;
	 break;
      case CLIP_PUSH_TRUE:
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 frame.ClipVar_sp_of_ClipFrame->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 1;
	 ++frame.ClipVar_sp_of_ClipFrame;
	 break;
      case CLIP_PUSH_FALSE:
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 frame.ClipVar_sp_of_ClipFrame->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
	 ++frame.ClipVar_sp_of_ClipFrame;
	 break;
      case CLIP_MEMVAR_PUBLIC:
	 l = GET_LONG(pp);
	 _clip_memvar_public(ClipMachineMemory, l);
	 break;
      case CLIP_MEMVAR_SPACE:
	 l = GET_LONG(pp);
	 l1 = GET_LONG(pp);
	 _clip_memvar_space(ClipMachineMemory, _clip_space(ClipMachineMemory, l), l1, 1);
	 break;
      case CLIP_MEMVAR_PRIVATE:
	 l = GET_LONG(pp);
	 _clip_memvar_private(ClipMachineMemory, l);
	 break;
      case CLIP_MEMVAR_PUBLIC_POP:
	 l = _clip_pop_hash(ClipMachineMemory);
	 _clip_memvar_public(ClipMachineMemory, l);
	 break;
      case CLIP_MEMVAR_SPACE_POP:
	 l = GET_LONG(pp);
	 l1 = _clip_pop_hash(ClipMachineMemory);
	 _clip_memvar_space(ClipMachineMemory, _clip_space(ClipMachineMemory, l), l1, 1);
	 break;
      case CLIP_MEMVAR_PRIVATE_POP:
	 l = _clip_pop_hash(ClipMachineMemory);
/*_clip_memvar_private(ClipMachineMemory, l);*/
	 _clip_add_private(ClipMachineMemory, l);
	 break;
      case CLIP_MEMVAR_PARAM:
	 l = GET_LONG(pp);
	 c = GET_BYTE(pp);
	 _clip_memvar_param(ClipMachineMemory, l, c);
	 break;
      case CLIP_PUSH_PARAM:
	 c = GET_BYTE(pp);
	 s = GET_SHORT(pp);
	 _clip_param(ClipMachineMemory, c, s);
	 break;
      case CLIP_PUSH_LOCAL:
	 s = GET_SHORT(pp);
	 if ((ret = _clip_push_local(ClipMachineMemory, s)))
	    goto _trap;
	 break;
      case CLIP_PUSH_REF_LOCAL:
	 s = GET_SHORT(pp);
#if 0
	 frame.sp->ClipRefVar_p_of_ClipVar.vp = _clip_ref_local(ClipMachineMemory, s);
#else
	 {
	    ClipVar *vp1 = _clip_ref_local(ClipMachineMemory, s);

	    frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	 }
#endif
	 break;
      case CLIP_PUSH_STATIC:
	 s = GET_SHORT(pp);
	 get_static(ClipMachineMemory, file, statics, modbeg, s, &vp);
	 if ((ret = _clip_push_static(ClipMachineMemory, vp)))
	    goto _trap;
	 break;
      case CLIP_PUSH_REF_STATIC:
	 s = GET_SHORT(pp);
	 get_static(ClipMachineMemory, file, statics, modbeg, s, &vp);
	 frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;
	 break;
      case CLIP_PUSH_MEMVAR:
	 l = GET_LONG(pp);
	 if ((ret = _clip_memvar(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_PUSH_FMEMVAR:
	 l = GET_LONG(pp);
	 if ((ret = _clip_fmemvar(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_PUSH_MEMVARF:
	 l = GET_LONG(pp);
	 if ((ret = _clip_memvarf(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_REF_FMEMVAR:
	 l = GET_LONG(pp);
	 if ((ret = _clip_ref_fmemvar(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_PUSH_REF_MEMVAR:
	 l = GET_LONG(pp);
#if 0
	 frame.sp->ClipRefVar_p_of_ClipVar.vp = _clip_ref_memvar(ClipMachineMemory, l);
#else
	 {
	    ClipVar *vp1 = _clip_ref_memvar(ClipMachineMemory, l);

	    frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	 }
#endif
	 break;
      case CLIP_PUSH_REF_MEMVAR_NOADD:
	 l = GET_LONG(pp);
	 {
	    ClipVar *vp1 = _clip_ref_memvar_noadd(ClipMachineMemory, l);

	    frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	 }
	 break;
      case CLIP_PUSH_PUBLIC:
	 l = GET_LONG(pp);
	 if ((ret = _clip_public(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_PUSH_REF_PUBLIC:
	 l = GET_LONG(pp);
#if 0
	 frame.sp->ClipRefVar_p_of_ClipVar.vp = _clip_ref_public(ClipMachineMemory, l);
#else
	 {
	    ClipVar *vp1 = _clip_ref_public(ClipMachineMemory, l);

	    frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	 }
#endif
	 break;
      case CLIP_REFMACRO:
	 if ((ret = _clip_refmacro(ClipMachineMemory)))
	    goto _trap;
	 break;
      case CLIP_MAKE_REF:
	 c = GET_BYTE(pp);
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_ref(ClipMachineMemory, vp, c)))
	    goto _trap;
	 break;
      case CLIP_UNREF_ARR:
	 _clip_unref_arr(ClipMachineMemory);
	 break;
      case CLIP_FIELD:
	 l = GET_LONG(pp);
	 l1 = GET_LONG(pp);
	 if ((ret = _clip_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_FIELD_POP:
	 l = GET_LONG(pp);
	 l1 = _clip_pop_hash(ClipMachineMemory);
	 if ((ret = _clip_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_FIELD_POP_NAME:
	 _clip_pop_fieldhash(ClipMachineMemory, &l1, &l);
	 if ((ret = _clip_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_PUSH_AREA:
	 l = GET_LONG(pp);
	 if ((ret = _clip_push_area(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_PUSH_AREA_POP:
	 l = _clip_pop_hash(ClipMachineMemory);
	 if ((ret = _clip_push_area(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_POP_AREA:
	 if ((ret = _clip_pop_area(ClipMachineMemory)))
	    goto _trap;
	 break;
      case CLIP_FUNC:
	 c = GET_BYTE(pp);
	 l = GET_LONG(pp);
	 if ((ret = _clip_func_hash(ClipMachineMemory, l, c, 0, reflocals)))
	    goto _trap;
	 break;
      case CLIP_FUNCR:
	 c = GET_BYTE(pp);
	 l = GET_LONG(pp);
	 if ((ret = _clip_func_hash(ClipMachineMemory, l, c, 1, reflocals)))
	    goto _trap;
	 break;
      case CLIP_PROC:
	 c = GET_BYTE(pp);
	 l = GET_LONG(pp);
	 if ((ret = _clip_proc_hash(ClipMachineMemory, l, c, 0, reflocals)))
	    goto _trap;
	 break;
      case CLIP_PROCR:
	 c = GET_BYTE(pp);
	 l = GET_LONG(pp);
	 if ((ret = _clip_proc_hash(ClipMachineMemory, l, c, 1, reflocals)))
	    goto _trap;
	 break;
      case CLIP_SFUNC:
	 c = GET_BYTE(pp);
	 s = GET_SHORT(pp);
	 bl.file_of_ClipBlock = file;
	 get_func(modbeg, s, &bl.function_of_ClipBlock);
	 if ((ret = _clip_code_func(ClipMachineMemory, &bl, c, 0, reflocals)))
	    goto _trap;
	 break;
      case CLIP_SFUNCR:
	 c = GET_BYTE(pp);
	 s = GET_SHORT(pp);
	 bl.file_of_ClipBlock = file;
	 get_func(modbeg, s, &bl.function_of_ClipBlock);
	 if ((ret = _clip_code_func(ClipMachineMemory, &bl, c, 1, reflocals)))
	    goto _trap;
	 break;
      case CLIP_SPROC:
	 c = GET_BYTE(pp);
	 s = GET_SHORT(pp);
	 bl.file_of_ClipBlock = file;
	 get_func(modbeg, s, &bl.function_of_ClipBlock);
	 if ((ret = _clip_code_proc(ClipMachineMemory, &bl, c, 0, reflocals)))
	    goto _trap;
	 break;
      case CLIP_SPROCR:
	 c = GET_BYTE(pp);
	 s = GET_SHORT(pp);
	 bl.file_of_ClipBlock = file;
	 get_func(modbeg, s, &bl.function_of_ClipBlock);
	 if ((ret = _clip_code_proc(ClipMachineMemory, &bl, c, 1, reflocals)))
	    goto _trap;
	 break;
      case CLIP_ASSIGN:
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_assign(ClipMachineMemory, vp)))
	    goto _trap;
	 break;
      case CLIP_REF_DESTROY:
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 /*if (vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MREF_ClipFlags) */
	 _clip_destroy(ClipMachineMemory, vp);
			/*_clip_destroy(ClipMachineMemory, vp);*/
	 break;
      case CLIP_MACROASSIGN:
	 c = GET_BYTE(pp);
	 c1 = GET_BYTE(pp);
	 if ((ret = _clip_macroassign(ClipMachineMemory, c, c1 & 1, c1 & 2)))
	    goto _trap;
	 break;
      case CLIP_REFASSIGN:
	 c = GET_BYTE(pp);
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_refassign(ClipMachineMemory, vp, c)))
	    goto _trap;
	 break;
      case CLIP_UNREF:
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 _clip_destroy(ClipMachineMemory, vp);
	 break;
      case CLIP_IASSIGN:
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_iassign(ClipMachineMemory, vp)))
	    goto _trap;
	 break;
      case CLIP_OPASSIGN:
	 c = GET_BYTE(pp);
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_opassign(ClipMachineMemory, vp, c)))
	    goto _trap;
	 break;
      case CLIP_OPIASSIGN:
	 c = GET_BYTE(pp);
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_opiassign(ClipMachineMemory, vp, c)))
	    goto _trap;
	 break;
      case CLIP_ASSIGN_FIELD:
	 l = GET_LONG(pp);
	 l1 = GET_LONG(pp);
	 if ((ret = _clip_assign_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_IASSIGN_FIELD:
	 l = GET_LONG(pp);
	 l1 = GET_LONG(pp);
	 if ((ret = _clip_iassign_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_ASSIGN_FIELD_POP:
	 l = GET_LONG(pp);
	 l1 = _clip_pop_hash(ClipMachineMemory);
	 if ((ret = _clip_assign_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_IASSIGN_FIELD_POP:
	 l = GET_LONG(pp);
	 l1 = _clip_pop_hash(ClipMachineMemory);
	 if ((ret = _clip_iassign_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_ASSIGN_PFIELD:
	 l = _clip_pop_hash(ClipMachineMemory);
	 l1 = GET_LONG(pp);
	 if ((ret = _clip_assign_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_IASSIGN_PFIELD:
	 l = _clip_pop_hash(ClipMachineMemory);
	 l1 = GET_LONG(pp);
	 if ((ret = _clip_iassign_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_ASSIGN_PFIELD_POP:
	 l = _clip_pop_hash(ClipMachineMemory);
	 l1 = _clip_pop_hash(ClipMachineMemory);
	 if ((ret = _clip_assign_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_IASSIGN_PFIELD_POP:
	 l = _clip_pop_hash(ClipMachineMemory);
	 l1 = _clip_pop_hash(ClipMachineMemory);
	 if ((ret = _clip_iassign_field(ClipMachineMemory, l, l1)))
	    goto _trap;
	 break;
      case CLIP_FM_ASSIGN:
	 l = GET_LONG(pp);
	 if ((ret = _clip_fm_assign(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_FM_IASSIGN:
	 l = GET_LONG(pp);
	 if ((ret = _clip_fm_iassign(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_ARRAY:
	 l = GET_LONG(pp);
	 _clip_sarray(ClipMachineMemory, l);
	 break;
      case CLIP_DIMARRAY:
	 l = GET_LONG(pp);
	 _clip_dimarray(ClipMachineMemory, l);
	 break;
      case CLIP_NEWARRAY:
	 l = GET_LONG(pp);
	 _clip_vnewarray(ClipMachineMemory, l, (long *) pp);
	 pp += l * sizeof(long);

	 break;
      case CLIP_GET:
	 l = GET_LONG(pp);
	 if ((ret = _clip_get(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_SET:
	 l = GET_LONG(pp);
	 if ((ret = _clip_set(ClipMachineMemory, l)))
	    goto _trap;
	 break;
      case CLIP_RETURN_POP:
	 _clip_return(ClipMachineMemory);
      case CLIP_RETURN:
	 goto _return;
      case CLIP_STORE:
	 c1 = GET_BYTE(pp);
	 c = GET_BYTE(pp);
	 if ((ret = _clip_store(ClipMachineMemory, c, c1)))
	    goto _trap;
	 break;
      case CLIP_ISTORE:
	 c1 = GET_BYTE(pp);
	 c = GET_BYTE(pp);
	 if ((ret = _clip_istore(ClipMachineMemory, c, c1)))
	    goto _trap;
	 break;
      case CLIP_OPSTORE:
	 c = GET_BYTE(pp);
	 c1 = GET_BYTE(pp);
	 if ((ret = _clip_opstore(ClipMachineMemory, c1, c)))
	    goto _trap;
	 break;
      case CLIP_OPISTORE:
	 c = GET_BYTE(pp);
	 c1 = GET_BYTE(pp);
	 if ((ret = _clip_opistore(ClipMachineMemory, c1, c)))
	    goto _trap;
	 break;
      case CLIP_FETCH:
	 c = GET_BYTE(pp);
	 if ((ret = _clip_fetch(ClipMachineMemory, c)))
	    goto _trap;
	 break;
      case CLIP_FETCHREF:
	 c = GET_BYTE(pp);
#if 0
	 frame.sp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = _clip_fetchref(ClipMachineMemory, c);
#else
	 {
	    ClipVar *vp1 = _clip_fetchref(ClipMachineMemory, c);

	    /*printf("%p\n", vp1); */
	    frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp1;
	 }
#endif
	 if (!frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar)
	    goto _trap;
	 break;
      case CLIP_IFETCH:
	 c = GET_BYTE(pp);
	 if ((ret = _clip_ifetch(ClipMachineMemory, c)))
	    goto _trap;
	 break;
      case CLIP_CALL:
	 c = GET_BYTE(pp);
	 l = GET_LONG(pp);
	 if ((ret = _clip_call(ClipMachineMemory, c, l)))
	    goto _trap;
	 break;
      case CLIP_PUSH_CODE:
	 s = GET_SHORT(pp);
	 c = GET_BYTE(pp);
	 c1 = GET_BYTE(pp);

	 vp = NEW(ClipVar);
	 vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
	 vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 vp->ClipType_t_of_ClipVar.count_of_ClipType = 1;
	 vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar = NEW(ClipBlock);

	 get_func(modbeg, s, &(vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar->function_of_ClipBlock));
	 vp->ClipCodeVar_c_of_ClipVar.union_of_ClipCodeVar.ClipBlock_block_of_ClipCodeVar->file_of_ClipBlock = file;

	 if (c)
	 {
	    int nlocals = c;

	    ClipVarFrame *localvars = (ClipVarFrame *) calloc(1,
							      sizeof(ClipVarFrame) + nlocals * sizeof(ClipVar));

	    localvars->ClipVar_vars_of_ClipVarFrame = (ClipVar *) (localvars + 1);
	    memcpy(localvars->ClipVar_vars_of_ClipVarFrame, ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - nlocals, nlocals * sizeof(ClipVar));
	    localvars->refcount_of_ClipVarFrame = 1;
	    localvars->size_of_ClipVarFrame = nlocals;
	    vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = localvars;
	    ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame -= nlocals;
	 }
	 else if (!c1 && reflocals && reflocals->refcount_of_ClipVarFrame)
	 {
	    reflocals->refcount_of_ClipVarFrame++;
	    vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = reflocals;
	 }
	 else
	    vp->ClipCodeVar_c_of_ClipVar.ClipVarFrame_uplocals_of_ClipCodeVar = 0;

	 file->refCount_of_ClipFile++;

	 CLEAR_CLIPVAR(frame.ClipVar_sp_of_ClipFrame);
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = PCODE_type_of_ClipVarType;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	 frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = vp;

	 frame.ClipVar_sp_of_ClipFrame++;
	 CLIP_CHECK_STACK;

	 break;
      case CLIP_MACRO:
	 if ((ret = _clip_macro(ClipMachineMemory)))
	    goto _trap;
	 break;
      case CLIP_PCOUNT:
	 _clip_pcount(ClipMachineMemory);
	 break;
      case CLIP_PSHIFT:
	 _clip_pshift(ClipMachineMemory);
	 break;
      case CLIP_PARN:
	 if ((ret = _clip_parn(ClipMachineMemory)))
	    goto _trap;
	 break;
      case CLIP_FUNC_NAME:
	 c = GET_BYTE(pp);
	 if ((ret = _clip_func_name(ClipMachineMemory, c)))
	    goto _trap;
	 break;
      case CLIP_INCR:
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_incr(ClipMachineMemory, vp, 1, 0)))
	    goto _trap;
	 break;
      case CLIP_INCR_POST:
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_incr(ClipMachineMemory, vp, 1, 1)))
	    goto _trap;
	 break;
      case CLIP_DECR:
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_incr(ClipMachineMemory, vp, 0, 0)))
	    goto _trap;
	 break;
      case CLIP_DECR_POST:
	 vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	 if ((ret = _clip_incr(ClipMachineMemory, vp, 0, 1)))
	    goto _trap;
	 break;
      case CLIP_OP:
	 c = GET_BYTE(pp);
	 if ((ret = _clip_op(ClipMachineMemory, c)))
	    goto _trap;
	 break;
      case CLIP_NOT:
	 if ((ret = _clip_not(ClipMachineMemory)))
	    goto _trap;
	 break;
      case CLIP_COND:
	 s = GET_SHORT(pp);
	 if ((ret = _clip_cond(ClipMachineMemory, &i)))
	    goto _trap;
	 if (!i)
	    pp += s;
	 break;
      case CLIP_TCOND:
	 s = GET_SHORT(pp);
	 if ((ret = _clip_tcond(ClipMachineMemory, &i)))
	    goto _trap;
	 if (!i)
	    pp += s;
	 break;
      case CLIP_ITCOND:
	 s = GET_SHORT(pp);
	 if ((ret = _clip_tcond(ClipMachineMemory, &i)))
	    goto _trap;
	 if (i)
	    pp += s;
	 break;
      case CLIP_GOTO:
	 s = GET_SHORT(pp);
	 pp += s;
	 break;
      case CLIP_FORSTEP:
	 s = GET_SHORT(pp);
	 if ((ret = _clip_forstep(ClipMachineMemory, &i)))
	    goto _trap;
	 if (!i)
	    pp += s;
	 break;
      case CLIP_MAP_FIRST:
	 c = GET_BYTE(pp);
	 s = GET_SHORT(pp);
	 if ((ret = _clip_map_first(ClipMachineMemory, c, &i)))
	    goto _trap;
	 if (!i)
	    pp += s;
	 break;
      case CLIP_MAP_NEXT:
	 c = GET_BYTE(pp);
	 s = GET_SHORT(pp);
	 if ((ret = _clip_map_next(ClipMachineMemory, c, &i)))
	    goto _trap;
	 if (!i)
	    pp += s;
	 break;
      case CLIP_MINUS:
	 if ((ret = _clip_minus(ClipMachineMemory)))
	    goto _trap;
	 break;
      case CLIP_RESETTRAP:
	 trap_pp = 0;
	 break;
      case CLIP_SETTRAP:
	 s = GET_SHORT(pp);
	 trap_pp = pp + s;
	 break;
      case CLIP_RECOVER:
	 while (frame.ClipVar_sp_of_ClipFrame > frame.ClipVar_stack_of_ClipFrame)
	    _clip_destroy(ClipMachineMemory, --frame.ClipVar_sp_of_ClipFrame);
	 ret = 0;
	 break;
      case CLIP_USING:
	 if (ClipMachineMemory->trapVar)
	 {
	    vp = frame.ClipVar_sp_of_ClipFrame->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar;
	    *frame.ClipVar_sp_of_ClipFrame = *ClipMachineMemory->trapVar;
	    ++frame.ClipVar_sp_of_ClipFrame;
	    free(ClipMachineMemory->trapVar);
	    ClipMachineMemory->trapVar = 0;
	    if ((ret = _clip_assign(ClipMachineMemory, vp)))
	       goto _trap;
	 }
	 break;
      case CLIP_BREAK:
#if 0
			/*_clip_trap_str(ClipMachineMemory, filename, frame.line, "BREAK");*/
#else
	 vp = NEW(ClipVar);
	 _clip_trap_var(ClipMachineMemory, filename, frame.line_of_ClipFrame, vp);
#endif
	 ret = -1;
	 goto _trap;
      case CLIP_BREAK_EXPR:
	 vp = NEW(ClipVar);
	 --frame.ClipVar_sp_of_ClipFrame;
	 *vp = *frame.ClipVar_sp_of_ClipFrame;
	 _clip_trap_var(ClipMachineMemory, filename, frame.line_of_ClipFrame, vp);
	 ret = -1;
	 goto _trap;
      case CLIP_SWAP:
	 c = GET_BYTE(pp);
	 _clip_swap(ClipMachineMemory, c);
	 break;
      case CLIP_PUSH_HASH:
	 l = GET_LONG(pp);
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.len_of_ClipType = 10;
	 frame.ClipVar_sp_of_ClipFrame->ClipType_t_of_ClipVar.dec_of_ClipType = 0;
	 frame.ClipVar_sp_of_ClipFrame->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = l;
	 frame.ClipVar_sp_of_ClipFrame++;
	 CLIP_CHECK_STACK;
	 break;
      case CLIP_CALC_HASH:
	 _clip_calc_hash(ClipMachineMemory);
	 break;
      case CLIP_CALC_HASH2:
	 _clip_calc_hash2(ClipMachineMemory, 1);
	 break;
      case CLIP_PUSH_LOCALE:
	 _clip_push_locale(ClipMachineMemory);
	 break;
      case CLIP_RDDFIELD:
	 s = GET_SHORT(pp);
	 s1 = GET_SHORT(pp);
	 _clip_rddfield(ClipMachineMemory, s, s1);
	 break;
      case CLIP_CATSTR:
	 s = GET_SHORT(pp);
	 _clip_catstr(ClipMachineMemory, s);
	 break;
      case CLIP_QUOT:
	 _clip_quot(ClipMachineMemory);
	 break;
      case CLIP_SWITCH:
	 s = GET_SHORT(pp);	/* label num */
	 l = _clip_pop_shash(ClipMachineMemory);	/* hash */
	 {
	    short other = GET_SHORT(pp);

	    long *lp = (long *) pp;

	    short *jmps = (short *) (pp + s * sizeof(long));

	    int n = search_long(lp, s, l);

	    if (n < 0)
	       pp += other;
	    else
	       pp += GETSHORT(jmps + n);
	 }
	 break;
      default:
	 _clip_trap_printf(ClipMachineMemory, filename, frame.line_of_ClipFrame, "invalid bytecode %d", code);
	 _clip_call_errblock(ClipMachineMemory, 1);
	 ret = 1;
	 goto _trap;
      }
   }
   goto _return;
 _trap:
   if (trap_pp /*&& ret > 0 */ )
   {
      pp = trap_pp;
      goto cont;
   }
   _clip_trap(ClipMachineMemory, filename, frame.line_of_ClipFrame);
   /*ret = 1; */
 _return:
   if (_clip_profiler)
      _clip_stop_profiler(ClipMachineMemory);
   if (local_locals)
      _clip_destroy_locals(ClipMachineMemory);
   _clip_resume(ClipMachineMemory, nlocals, nreflocals);
#if 0
   _clip_vresume(ClipMachineMemory, nlocals, locals);
#endif
#if 0
   _clip_vresume(ClipMachineMemory, nparams, params);
#endif
#if 0
   if (nreflocals)
      _clip_vresume(ClipMachineMemory, nreflocals, reflocals);
#endif
/*_clip_vremove_privates(ClipMachineMemory, nprivates, privates);*/

#ifdef OS_MINGW
   free(stack);
#endif
   return ret;
}
