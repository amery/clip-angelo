int
clip_ASCAN(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *bp = _clip_par(ClipMachineMemory, 2);

   int start = _clip_parni(ClipMachineMemory, 3) - 1;

   int count = _clip_parni(ClipMachineMemory, 4);

   int back = _clip_parl(ClipMachineMemory, 5);

   int c, i, r, code = 0, no = 0, end;

   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType || !bp)
   {
#if 0
      return EG_ARG;
#else
      _clip_retni(ClipMachineMemory, 0);
      return 0;
#endif
   }
   if (bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType || bp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType)
      code = 1;

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      if (start >= c)
	 goto _ret;
      if (start < 0)
	 start = 0;
      if (count <= 0 || count > (c - start))
	 count = c - start;
      end = start + count;

      if (code)
      {
	 if (back)
	    i = end - 1;
	 else
	    i = start;
	 for (;;)
	 {
	    ClipVar res, *app;

	    if (back)
	    {
	       if (i <= start)
		  break;
	    }
	    else
	    {
	       if (i >= end)
		  break;
	    }

	    app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;
	    memset(&res, 0, sizeof(res));

	    r = _clip_eval(ClipMachineMemory, bp, 1, app, &res);
	    /*
	       if (app->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType != F_MREF)
	       app->ClipRefVar_p_of_ClipVar.vp->ClipType_t_of_ClipVar.count_of_ClipType --;
	     */
	    if (!r && res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType && res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	       no = i + 1;
	    _clip_destroy(ClipMachineMemory, &res);

	    if (r)
	       return r;
	    if (no)
	       break;

	    if (back)
	       i--;
	    else
	       i++;
	 }
      }
      else
      {
	 if (back)
	    i = end - 1;
	 else
	    i = start;
	 for (;;)
	 {
	    ClipVar *app;

	    int res = 0;

	    if (back)
	    {
	       if (i <= start)
		  break;
	    }
	    else
	    {
	       if (i >= end)
		  break;
	    }

	    app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	    r = _clip_cmp(ClipMachineMemory, app, bp, &res, 1);
	    if (r)
	       return r;
	    if (!res)
	    {
	       no = i + 1;
	       break;
	    }
	    if (back)
	       i--;
	    else
	       i++;
	 }
      }

   }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
   {
      c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
      if (start >= c)
	 goto _ret;
      if (start < 0)
	 start = 0;
      if (count <= 0 || count > (c - start))
	 count = c - start;
      end = start + count;

      if (code)
      {
	 if (back)
	    i = end - 1;
	 else
	    i = start;
	 for (;;)
	 {
	    ClipVar res, *app;

	    if (back)
	    {
	       if (i <= start)
		  break;
	    }
	    else
	    {
	       if (i >= end)
		  break;
	    }

	    app = &ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl;
	    memset(&res, 0, sizeof(res));

	    r = _clip_eval(ClipMachineMemory, bp, 1, app, &res);
	    if (!r && res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType && res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	       no = i + 1;
	    _clip_destroy(ClipMachineMemory, &res);
	    if (r)
	       return r;
	    if (no)
	       break;

	    if (back)
	       i--;
	    else
	       i++;
	 }
      }
      else
      {
	 end = start + count;

	 if (code)
	 {
	    if (back)
	       i = end - 1;
	    else
	       i = start;
	    for (;;)
	    {
	       ClipVar *app;

	       int res = 0;

	       if (back)
	       {
		  if (i <= start)
		     break;
	       }
	       else
	       {
		  if (i >= end)
		     break;
	       }

	       app = &ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[i].ClipVar_var_of_ClipVarEl;

	       r = _clip_cmp(ClipMachineMemory, app, bp, &res, 1);
	       if (r)
		  return r;
	       if (!res)
	       {
		  no = i + 1;
		  break;
	       }

	       if (back)
		  i--;
	       else
		  i++;
	    }
	 }
      }
   }

 _ret:
   _clip_retni(ClipMachineMemory, no);

   return 0;
}
