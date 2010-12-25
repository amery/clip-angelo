int
rdd_rawread(ClipMachine * ClipMachineMemory, RDD_DATA * rd, void *buf, const char *__PROC__)
{
   return rdd_read(ClipMachineMemory, &rd->file, rd->hdrsize + rd->recsize * (rd->recno - 1), rd->recsize, buf, __PROC__);
}
