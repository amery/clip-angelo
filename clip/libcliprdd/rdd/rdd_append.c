int
rdd_append(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *neterr, const char *__PROC__)
{
   int r, er;

   int lastrec;

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rdd_event(ClipMachineMemory, EVENT_APPEND, rd->area, 0, NULL, &r, __PROC__)))
      return er;
   if (!r)
      return 0;
   if ((er = rdd_lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;
   if ((er = rd->vtbl->rlock(ClipMachineMemory, rd, lastrec + 1, &r, __PROC__)))
      return er;
   *neterr = 0;
   if (!r)
   {				// neterr()
      *neterr = 1;
      return 0;
   }
   if ((er = rd->vtbl->append(ClipMachineMemory, rd, __PROC__)))
      return er;
   if (rd->filter && rd->filter->rmap)
   {
      unsigned int bytes = ((lastrec + 1) >> 5) + 1;

      int fok;

      if (((lastrec) >> 5) + 1 < bytes)
      {
	 rd->filter->rmap = realloc(rd->filter->rmap, bytes << 2);
	 rd->filter->size = lastrec;
      }
      if ((er = rdd_calcfilter(ClipMachineMemory, rd, &fok, __PROC__)))
	 return er;
      if (fok)
      {
	 _rm_setbit(rd->filter->rmap, rd->filter->size, rd->recno);
      }
      else
      {
	 _rm_clrbit(rd->filter->rmap, rd->filter->size, rd->recno);
      }
   }
   rd->newrec = 1;
   return 0;
}
