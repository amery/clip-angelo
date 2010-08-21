int
rdd_recall(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int i, r, er;

   RDD_ORDER *ro;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rdd_event(ClipMachineMemory, EVENT_RECALL, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;
   if (rd->eof)
      return 0;

   if ((er = rd->vtbl->rlocked(ClipMachineMemory, rd, rd->recno, &r, __PROC__)))
      return er;
   if (!r && !rd->flocked)
      return rdd_err(ClipMachineMemory, EG_UNLOCKED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (!rd->newrec)
   {
      for (i = 0; i < rd->ords_opened; i++)
      {
	 ro = rd->orders[i];
	 if (!ro->custom)
	 {
	    if ((ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) && ro->simpexpr)
	       if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	    if ((er = ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__)))
	       goto unlock;
	    ro->index->updated = 1;
	 }
      }
      rd->eof = 0;
   }
   if ((er = rd->vtbl->recall(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (!rd->newrec)
   {
      for (i = 0; i < rd->ords_opened; i++)
      {
	 RDD_ORDER *ro = rd->orders[i];

	 ClipVar vv, *vp;

	 if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
	       goto unlock;
	    vp = _clip_vptr(&vv);
	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	    {
	       er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	       goto unlock;
	    }
	    if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMachineMemory, &vv);
	       if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
		  goto unlock;
	       continue;
	    }
	    _clip_destroy(ClipMachineMemory, &vv);
	 }
	 if (!ro->custom)
	 {
	    if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	       goto unlock;
	    if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
	       goto unlock;
	 }
      }
   }
   if (rd->filter && rd->filter->rmap)
   {
      int fok;

      if ((er = rdd_calcfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	 return er;
      if (fok)
	 _rm_setbit(rd->filter->rmap, rd->filter->size, rd->recno);
      else
	 _rm_clrbit(rd->filter->rmap, rd->filter->size, rd->recno);
   }
   return 0;
 unlock:
   if (!rd->newrec)
   {
      for (i = 0; i < rd->ords_opened; i++)
      {
	 ro = rd->orders[i];
	 if (!ro->custom)
	 {
	    ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
	 }
      }
   }
   return er;
}
