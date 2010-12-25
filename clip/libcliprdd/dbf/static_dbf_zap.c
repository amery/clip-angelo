static int
dbf_zap(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   DBF_HEADER hdr;

   char end = 0x1A;

   int er;

   if (rd->readonly)
      return rdd_err(ClipMachineMemory, EG_READONLY, 0, __FILE__, __LINE__, __PROC__, er_notpermitted);
   rd->updated = 1;
   if ((er = rdd_read(ClipMachineMemory, &rd->file, 0, sizeof(DBF_HEADER), &hdr, __PROC__)))
      return er;
   _rdd_put_uint(hdr.recs, 0);
   if ((er = rdd_write(ClipMachineMemory, &rd->file, 0, sizeof(DBF_HEADER), &hdr, __PROC__)))
      return er;
   if ((er = rdd_trunc(ClipMachineMemory, &rd->file, _rdd_ushort(hdr.hdrsize) + 1, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &rd->file, _rdd_ushort(hdr.hdrsize), 1, &end, __PROC__)))
      return er;
   return 0;
}
