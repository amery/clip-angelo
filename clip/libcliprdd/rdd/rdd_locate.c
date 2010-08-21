int
rdd_locate(ClipMachine * ClipMachineMemory, RDD_DATA * rd, char *cfor, ClipVar * fexpr, ClipVar * wexpr, ClipVar * vnext, ClipVar * vrec, ClipVar * vrest, int *found, const char *__PROC__)
{
   RDD_FILTER *old = NULL;

   int next = -1, rec = 0, rest = 0, r = 0, er;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   *found = 0;
   if (!fexpr || (fexpr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType && fexpr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      fexpr = 0;

   if (!wexpr || (wexpr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CCODE_type_of_ClipVarType && wexpr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != PCODE_type_of_ClipVarType))
      wexpr = 0;

   if (vnext && vnext->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
   {
      next = _clip_double(vnext);
      next = next ? next - 1 : 0;
   }
   if (vrec && vrec->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
   {
      rec = _clip_double(vrec);
   }
   if (vrest && vrest->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
      rest = vrest->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
   else if (fexpr)
      rest = 0;
   else
      rest = 1;

   if (!fexpr && !wexpr && !rd->locate_filter && !rd->locate_wpar)
      return 0;

   if (!wexpr)
   {
      if (!rest)
      {
	 if (rec)
	    er = rdd_goto(ClipMachineMemory, rd, rec, __PROC__);
	 else
	    er = rdd_gotop(ClipMachineMemory, rd, __PROC__);
	 if (er)
	    return er;
	 rd->locate_next = next;
      }
   }

   if (fexpr)
   {
      ClipVar *dest = (ClipVar *) calloc(1, sizeof(ClipVar));

      if (rd->locate_wpar)
      {
	 _clip_destroy(ClipMachineMemory, rd->locate_wpar);
	 free(rd->locate_wpar);
      }
      if (rd->locate_filter)
	 if ((er = rdd_destroyfilter(ClipMachineMemory, rd->locate_filter, __PROC__)))
	    return er;
      if ((er = rdd_createfilter(ClipMachineMemory, rd, &rd->locate_filter, fexpr, cfor, NULL, 0, __PROC__)))
	 return er;
      if (rd->filter)
	 if ((er = rm_intersect(ClipMachineMemory, rd->locate_filter, rd->filter, __PROC__)))
	    return er;
      if (wexpr)
      {
	 _clip_clone(ClipMachineMemory, dest, wexpr);
	 rd->locate_wpar = dest;
      }
      else
      {
	 free(dest);
	 rd->locate_wpar = NULL;
      }
      rd->locate_next = next;
   }

   if (!rd->locate_wpar)
   {
      if (rd->filter)
      {
	 old = rd->filter;
	 old->active = 0;
      }
      rd->filter = rd->locate_filter;
      rd->filter->active = 1;
   }

   while (1)
   {
      int wcond = 1;

      if ((er = rdd_eof(ClipMachineMemory, rd, &r, __PROC__)))
	 return er;

      if (r || !rd->locate_next)
      {
	 /* wa->locate_next = -1; */
	 break;
      }

      if (rd->locate_wpar)
      {
	 ClipVar data;

	 _clip_eval(ClipMachineMemory, rd->locate_wpar, 0, 0, &data);

	 if (data.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	    break;
	 else
	    wcond = data.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;

	 _clip_destroy(ClipMachineMemory, &data);
      }

      if (!wcond)
	 break;

      if (rd->locate_wpar)
      {
	 ClipVar data;

	 _clip_eval(ClipMachineMemory, &rd->locate_filter->fps->bfilter, 0, 0, &data);

	 if (data.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	    break;
	 else
	    *found = data.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;

	 _clip_destroy(ClipMachineMemory, &data);
      }
      else
      {
	 if ((er = rdd_calcfilter(ClipMachineMemory, rd, found, __PROC__)))
	    return er;
      }

      if (*found)
	 break;

      if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
	 return er;

      if (rd->locate_next > 0)
	 rd->locate_next--;
   }

   if (!rd->locate_wpar)
   {
      if (rd->locate_filter)
	 rd->locate_filter->active = 0;
      rd->filter = NULL;
      if (old)
      {
	 rd->filter = old;
	 rd->filter->active = 1;
      }
   }
   return 0;
}
