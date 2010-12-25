static int
dbf_close(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int i;

   unsigned int lastrec;

   DBF_HEADER hdr;

   char end = 0x1A;

   time_t ltm;

   struct tm *date;

   int er;

   for (i = 0; i < rd->idxs_opened; i++)
   {
      if ((er = rd->indices[i]->vtbl->close(ClipMachineMemory, rd, rd->indices[i], __PROC__)))
	 return er;
   }
   if (rd->memo)
   {
      if ((er = rd->memo->vtbl->close(ClipMachineMemory, rd, rd->memo, __PROC__)))
	 return er;
      rd->memo = NULL;
   }
   if ((er = rdd_read(ClipMachineMemory, &rd->file, 0, sizeof(DBF_HEADER), &hdr, __PROC__)))
      return er;
   if ((er = dbf_lastrec(ClipMachineMemory, rd, (int *) (&lastrec), __PROC__)))
      return er;
   if (!rd->readonly && rd->updated)
   {
      if (!rd->shared)
      {
	 if ((er = rdd_trunc(ClipMachineMemory, &rd->file, _rdd_ushort(hdr.hdrsize) + _rdd_ushort(hdr.recsize) * lastrec + 1, __PROC__)))
	    return er;
      }
      if ((er = rdd_write(ClipMachineMemory, &rd->file, _rdd_ushort(hdr.hdrsize) + _rdd_ushort(hdr.recsize) * lastrec, 1, &end, __PROC__)))
	 return er;
   }
   if (rd->updated)
   {
      ltm = time(NULL);
      date = gmtime(&ltm);
      hdr.yy = date->tm_year < 100 ? date->tm_year : date->tm_year - 100;
      hdr.mm = date->tm_mon + 1;
      hdr.dd = date->tm_mday;
      if ((er = rdd_write(ClipMachineMemory, &rd->file, 1, 3, &hdr.yy, __PROC__)))
	 return er;
   }
/*
	if(_clip_unlock(ClipMachineMemory,rd->file.filehash,rd->file.fd,0x7fffffff,0))
		return rdd_err(ClipMachineMemory,EG_CLOSE,errno,__FILE__,__LINE__,__PROC__,
			er_unlock);
*/
   return 0;
}
