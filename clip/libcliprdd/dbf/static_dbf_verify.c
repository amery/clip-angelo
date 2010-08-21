static int
dbf_verify(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *r, const char *__PROC__)
{
   void *buf = malloc(rd->recsize);

   int er;

   if (!rd->valid)
   {
      if ((er = dbf_getrecord(ClipMachineMemory, rd, __PROC__)))
	 return er;
      *r = 1;
      return 0;
   }
   if ((er = rdd_read(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, buf, __PROC__)))
      return er;
   *r = !memcmp(rd->record, buf, rd->recsize);
   return 0;
}
