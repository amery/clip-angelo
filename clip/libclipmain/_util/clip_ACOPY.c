int
clip_ACOPY(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *dp = _clip_par(ClipMachineMemory, 2);

   ClipVar *tp = _clip_spar(ClipMachineMemory, 2);

   ClipVar *rp;

   int start = _clip_parni(ClipMachineMemory, 3) - 1;

   int count;

   int dstart = _clip_parni(ClipMachineMemory, 5) - 1;

   int c, i, end, r, j;

   if (!ap || !dp)
      return EG_ARG;

   if (ClipMachineMemory->argc < 4)
      count = -1;
   else
      count = _clip_parni(ClipMachineMemory, 4);

   rp = RETPTR(ClipMachineMemory);

   if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType && dp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == ARRAY_type_of_ClipVarType)
   {
      c = ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      if (start >= c)
	 return 0;
      if (start < 0)
	 start = 0;
      if (count < 0 || count > (c - start))
	 count = c - start;
      c = dp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
      if (dstart >= c)
	 return 0;		/*EG_ARG; */
      if (dstart < 0)
	 dstart = 0;
      if (count > (c - dstart))
	 count = c - dstart;

      for (i = start, j = dstart, end = start + count; i < end; ++i, ++j)
      {
	 ClipVar *app = ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i;

	 ClipVar *dpp = dp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + j;

	 _clip_destroy(ClipMachineMemory, dpp);
	 r = _clip_clone(ClipMachineMemory, dpp, app);
	 if (r)
	    return r;
      }
   }
   else if (ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType && dp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == MAP_type_of_ClipVarType)
   {
      c = ap->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
      if (start >= c)
	 return 0;
      if (start < 0)
	 start = 0;
      if (count < 0 || count > (c - start))
	 count = c - start;
      c = dp->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar;
      if (start >= c)
	 return 0;
      if (dstart < 0)
	 dstart = 0;
      if (count > (c - dstart))
	 count = c - dstart;

      for (i = start, j = dstart, end = start + count; i < end; ++i, ++j)
      {
	 ClipVarEl *app = ap->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + i;

	 ClipVarEl *dpp = dp->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar + j;

	 _clip_destroy(ClipMachineMemory, &dpp->ClipVar_var_of_ClipVarEl);
	 r = _clip_clone(ClipMachineMemory, &dpp->ClipVar_var_of_ClipVarEl, &app->ClipVar_var_of_ClipVarEl);
	 if (r)
	    return r;
	 dpp->no_of_ClipVarEl = app->no_of_ClipVarEl;
      }
   }

   _clip_clone(ClipMachineMemory, rp, tp);
   return 0;
}
