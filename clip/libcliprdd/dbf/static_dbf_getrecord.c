static int
dbf_getrecord(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   rd->valid = 1;
   return rdd_read(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, rd->record, __PROC__);
}
