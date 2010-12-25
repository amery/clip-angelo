int
rdd_deleted(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *deleted, const char *__PROC__)
{
   int eof, er;

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      return er;
   if (eof)
      *deleted = 0;
   else if ((er = rd->vtbl->deleted(ClipMachineMemory, rd, deleted, __PROC__)))
      return er;
   return 0;
}
