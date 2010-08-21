int
rdd_clearindex(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int i, er;

   int structural = -1;

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   for (i = 0; i < rd->idxs_opened; i++)
   {
      if (rd->indices[i]->structural)
      {
	 structural = i;
      }
      else
      {
#ifdef HAVE_MMAN_H
	 if ((rd->indices[i]->file.md) != (caddr_t) - 1)
	 {
	    if (munmap(rd->indices[i]->file.md, rd->indices[i]->file.mapsize) == -1)
	       return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, rd->indices[i]->path);
	 }
#endif
	 if (_clip_close(ClipMachineMemory, rd->indices[i]->file.filehash, rd->indices[i]->file.fd) == -1)
	    return rdd_err(ClipMachineMemory, EG_CLOSE, errno, __FILE__, __LINE__, __PROC__, rd->indices[i]->path);
	 if ((er = rd->indices[i]->vtbl->close(ClipMachineMemory, rd, rd->indices[i], __PROC__)))
	    return er;
      }
   }
   if (structural == -1)
   {
      if (rd->indices)
	 free(rd->indices);
      rd->indices = NULL;
      if (rd->orders)
	 free(rd->orders);
      rd->orders = NULL;
      rd->idxs_opened = 0;
      rd->ords_opened = 0;
      rd->newrec = 0;
   }
   else
   {
      rd->indices[0] = rd->indices[structural];
      rd->indices = (RDD_INDEX **) realloc(rd->indices, sizeof(RDD_INDEX *));
      rd->idxs_opened = 1;
      rd->ords_opened = rd->indices[0]->norders;
      rd->orders = (RDD_ORDER **) realloc(rd->orders, rd->ords_opened * sizeof(RDD_ORDER *));
      for (i = 0; i < rd->ords_opened; i++)
      {
	 rd->orders[i] = rd->indices[0]->orders[i];
	 rd->orders[i]->orderno = i;
      }
   }
   rd->curord = -1;
   return 0;
}
