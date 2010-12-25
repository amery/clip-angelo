int
rdd_setvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int no, ClipVar * vp, const char *__PROC__)
{
   ClipVar *v = _clip_vptr(vp);

   RDD_ORDER *ro;

   int eof, i, r, er;

   unsigned char *recdup = NULL, *recbuf = NULL;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rdd_event(ClipMachineMemory, EVENT_PUT, rd->area, no + 1, v, &r, __PROC__)))
      return er;
   if (!r)
      return 0;

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      return er;
   if (eof)
   {
      return 0;
   }

   if ((er = rd->vtbl->rlocked(ClipMachineMemory, rd, rd->recno, &r, __PROC__)))
      return er;
   if (!r && !rd->flocked)
      return rdd_err(ClipMachineMemory, EG_UNLOCKED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if (!rd->valid)
   {
      if ((er = rd->vtbl->getrecord(ClipMachineMemory, rd, __PROC__)))
	 return er;
   }
   recdup = malloc(rd->recsize);
   memcpy(recdup, rd->record, rd->recsize);
   recbuf = malloc(rd->recsize);

   if ((er = rd->vtbl->setvalue(ClipMachineMemory, rd, no, v, 0, __PROC__)))
      goto unlock;

   memcpy(recbuf, recdup, rd->recsize);
   memcpy(recdup, rd->record, rd->recsize);
   memcpy(rd->record, recbuf, rd->recsize);

   if (!rd->newrec)
   {
      for (i = 0; i < rd->ords_opened; i++)
      {
	 ro = rd->orders[i];
	 if (!ro->custom)
	 {
	    if ((ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) && ro->simpexpr && (ro->simpfno != no))
	       continue;
	    if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
	       goto unlock;
	    if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
	       goto unlock;
	    if ((er = ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__)))
	       goto unlock;
	    ro->index->updated = 1;
	 }
      }
      rd->eof = 0;
   }

   memcpy(recbuf, recdup, rd->recsize);
   memcpy(recdup, rd->record, rd->recsize);
   memcpy(rd->record, recbuf, rd->recsize);
/*	if((er = rdd_childs(ClipMachineMemory,rd,__PROC__))) return er; */
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
	    if ((ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType) && ro->simpexpr && (ro->simpfno != no))
	       continue;
	    if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
	       goto unlock;
	    if (ro->simpexpr && ro->simpfno == no)
	    {
	       if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, v, __PROC__)))
		  goto unlock;
	    }
	    else
	    {
	       if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  goto unlock;
	    }
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
   if (recdup)
      free(recdup);
   if (recbuf)
      free(recbuf);
   return 0;
 unlock:
   if (!rd->newrec)
   {
      for (i = 0; i < rd->ords_opened; i++)
      {
	 ro = rd->orders[i];
	 if (!ro->custom)
	 {
	    if (ro->simpexpr && (ro->simpfno != no))
	       continue;
	    ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
	 }
      }
   }
   return er;
}
