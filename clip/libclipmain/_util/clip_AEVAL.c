int
clip_AEVAL(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *bp = _clip_par(ClipMachineMemory, 2);

   int start = _clip_parni(ClipMachineMemory, 3) - 1;

   int count = _clip_parni(ClipMachineMemory, 4);

   ClipVar *rp;

   int c, i, r;

   if (!ap)
      return 0;
   if (!bp || (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType && bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      return 0;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      if (start >= c)
	 return 0;
      if (start < 0)
	 start = 0;
      if (count < 0 || count > (c - start) || _clip_parinfo(ClipMachineMemory, 4) == UNDEF_type_of_ClipVarType)
	 count = c - start;

      for (i = start; i < start + count; ++i)
      {
	 ClipVar *nv = 0, *app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	 ClipVar res, stack[2];

	 unsigned l = 0;

	 memset(&res, 0, sizeof(ClipVar));
	 memset(stack, 0, sizeof(stack));

	 if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
	 {
	    //_clip_ref( ClipMachineMemory, app, 0 );
	    //--(ClipMachineMemory->fp->sp);
	    l = 1;
	    nv = NEW(ClipVar);

	    *nv = *app;

	    nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	    app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR_ClipFlags:F_MREF_ClipFlags */ ;
	    app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	    app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	    app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

	 }
	 stack[0] = *app;
	 stack[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 stack[1].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 stack[1].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = i + 1;

	 r = _clip_eval(ClipMachineMemory, bp, 2, stack, &res);
	 _clip_destroy(ClipMachineMemory, &res);
	 if (l)
	    --(nv->ClipType_t_of_ClipVar.count_of_ClipType);

	 if (r)
	    return r;
      }

      rp = RETPTR(ClipMachineMemory);
      ap = ARGPTR(ClipMachineMemory, 1);
      _clip_clone(ClipMachineMemory, rp, ap);
   }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
   {
      c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
      if (start >= c)
	 return 0;
      if (start < 0)
	 start = 0;
      if (count < 0 || count > (c - start))
	 count = c - start;

      for (i = 0; i < c; ++i)
      {
	 ClipVar *nv = 0, *app = &ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl;

	 long no = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].no_of_ClipVarEl;

	 ClipVar res, stack[2];

	 unsigned l = 0;

	 memset(&res, 0, sizeof(ClipVar));
	 memset(stack, 0, sizeof(stack));

	 if (!(app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MREF_ClipFlags))
	 {
	    //_clip_ref( ClipMachineMemory, app, 0 );
	    //--(ClipMachineMemory->fp->sp);
	    l = 1;
	    nv = NEW(ClipVar);

	    *nv = *app;

	    nv->ClipType_t_of_ClipVar.count_of_ClipType = 2;
	    app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags /*mptr?F_MPTR:F_MREF */ ;
	    app->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = nv;
	    app->ClipType_t_of_ClipVar.field_of_ClipType = 0;
	    app->ClipRefVar_p_of_ClipVar.ClipFieldDef_of_ClipRefVar = 0;

	 }
	 stack[0] = *app;
	 stack[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 stack[1].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	 stack[1].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = no;

	 r = _clip_eval(ClipMachineMemory, bp, 2, stack, &res);
	 _clip_destroy(ClipMachineMemory, &res);
	 if (l)
	    --(nv->ClipType_t_of_ClipVar.count_of_ClipType);
	 if (r)
	    return r;
      }

      rp = RETPTR(ClipMachineMemory);
      ap = ARGPTR(ClipMachineMemory, 1);
      _clip_clone(ClipMachineMemory, rp, ap);
   }

   return 0;
}
