int
rdd_reindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int i, er;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->shared)
      return rdd_err(ClipMachineMemory, EG_SHARED, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);

   for (i = 0; i < rd->idxs_opened; i++)
   {
      RDD_INDEX *ri = rd->indices[i];

      rd->indexing = ri->name;
      rd->reindexing = 1;
      if ((er = ri->vtbl->reindex(ClipMachineMemory, rd, ri, __PROC__)))
	 return er;
      rd->indexing = NULL;
      rd->reindexing = 0;
   }
   rd->newrec = 0;
   return rdd_gotop(ClipMachineMemory, rd, __PROC__);
}
