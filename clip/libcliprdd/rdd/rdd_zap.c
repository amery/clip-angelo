int
rdd_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int i, r, er;

   if ((er = rdd_event(ClipMachineMemory, EVENT_ZAP, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;

   if (rd->shared)
      return rdd_err(ClipMachineMemory, EG_SHARED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   if ((er = rd->vtbl->zap(ClipMachineMemory, rd, __PROC__)))
      return er;
   for (i = 0; i < rd->idxs_opened; i++)
      if ((er = rd->indices[i]->vtbl->zap(ClipMachineMemory, rd, rd->indices[i], __PROC__)))
	 return er;
   if (rd->memo)
      if ((er = rd->memo->vtbl->zap(ClipMachineMemory, rd->memo, __PROC__)))
	 return er;
   rd->bof = rd->v_bof = rd->eof = 1;
   rd->recno = 1;
   rd->newrec = 0;
   rd->valid = 0;
   return 0;
}
