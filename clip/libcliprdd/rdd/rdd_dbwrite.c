int
rdd_dbwrite(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * ap, const char *__PROC__)
{
   RDD_ORDER *ro;

   ClipVar *vp;

   ClipVar old;

   int eof;

   int i, j, r, er;

   int *fs = NULL;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;

   if ((er = rd->vtbl->rlocked(ClipMachineMemory, rd, rd->recno, &r, __PROC__)))
      return er;
   if (!r && !rd->flocked)
      return rdd_err(ClipMachineMemory, EG_UNLOCKED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      return er;
   if (eof)
      return 0;

   fs = calloc(rd->nfields, sizeof(int));
   for (i = 0; i < rd->nfields; i++)
   {
      vp = _clip_vptr(_clip_mget(ClipMachineMemory, ap, _clip_casehashword(rd->fields[i].name, strlen(rd->fields[i].name))));
      if (vp)
      {
	 if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, i, &old, __PROC__)))
	    goto err;
	 _clip_cmp(ClipMachineMemory, &old, vp, &(fs[i]), 1);
	 _clip_destroy(ClipMachineMemory, &old);
      }
   }
   if (!rd->newrec)
   {
      for (i = 0; i < rd->ords_opened; i++)
      {
	 ro = rd->orders[i];
	 if (!ro->custom)
	 {
	    if (ro->simpexpr)
	    {
	       for (j = 0; j < rd->nfields; j++)
		  if (fs[j] && ro->simpfno == j)
		     break;
	       if (j == rd->nfields)
		  continue;
	    }
	    if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
	       goto unlock;
	    if ((er = ro->vtbl->_wlock(ClipMachineMemory, ro, __PROC__)))
	       goto unlock;
	    if ((er = ro->vtbl->delkey(ClipMachineMemory, rd, ro, __PROC__)))
	       goto unlock;
	 }
      }
   }
   for (i = 0; i < rd->nfields; i++)
   {
      if (fs[i])
      {
	 vp = _clip_vptr(_clip_mget(ClipMachineMemory, ap, _clip_casehashword(rd->fields[i].name, strlen(rd->fields[i].name))));
	 if ((er = rd->vtbl->setvalue(ClipMachineMemory, rd, i, vp, 0, __PROC__)))
	    goto unlock;
      }
   }
   if (!rd->newrec)
   {
      for (i = 0; i < rd->ords_opened; i++)
      {
	 ro = rd->orders[i];
	 if (!ro->custom)
	 {
	    if (ro->simpexpr)
	    {
	       for (j = 0; j < rd->nfields; j++)
		  if (fs[j] && ro->simpfno == j)
		     break;
	       if (j == rd->nfields)
		  continue;
	    }
	    if ((er = rdd_event(ClipMachineMemory, EVENT_UPDATE, rd->area, 0, NULL, NULL, __PROC__)))
	       goto unlock;
	    if ((er = ro->vtbl->addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	       goto unlock;
	    if ((er = ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__)))
	       goto unlock;
	 }
      }
   }
   free(fs);
   fs = NULL;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
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
	 ro->vtbl->_ulock(ClipMachineMemory, ro, __PROC__);
      }
   }
 err:
   if (fs)
      free(fs);
   return er;
}
