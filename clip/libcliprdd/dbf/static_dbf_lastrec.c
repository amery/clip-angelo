static int
dbf_lastrec(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *lastrec, const char *__PROC__)
{
   char recs[4];

   int er;

   struct stat st;

   if (rd->shared)
   {
      fstat(rd->file.fd, &st);
      *lastrec = (st.st_size - rd->hdrsize) / rd->recsize;
   }
   else
   {
      if ((er = rdd_read(ClipMachineMemory, &rd->file, 4, 4, recs, __PROC__)))
	 return er;
      *lastrec = _rdd_uint((unsigned char *) recs);
   }
   return 0;
}
