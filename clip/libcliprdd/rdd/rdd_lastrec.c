int
rdd_lastrec(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *lastrec, const char *__PROC__)
{
   return rd->vtbl->lastrec(ClipMachineMemory, rd, lastrec, __PROC__);
}
