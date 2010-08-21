static int
dbt_zap(ClipMachine * ClipMachineMemory, RDD_MEMO * rm, const char *__PROC__)
{
   DBT_HEADER hdr;

   int er;

   if ((er = rdd_trunc(ClipMachineMemory, &rm->file, sizeof(DBT_HEADER), __PROC__)))
      return er;
   if ((er = rdd_read(ClipMachineMemory, &rm->file, 0, sizeof(DBT_HEADER), &hdr, __PROC__)))
      return er;
   _rdd_put_uint((unsigned char *) hdr.fuu, 1);
   return rdd_write(ClipMachineMemory, &rm->file, 0, sizeof(DBT_HEADER), &hdr, __PROC__);
}
