static int
dbf_setrecord(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   rd->changed = 0;
   return rdd_write(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, rd->record, __PROC__);
}
