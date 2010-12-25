int
rdd_rlock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, int *r, const char *__PROC__)
{
   rd->valid = 0;
   if (rec <= 0)
      return 0;
   return rd->vtbl->rlock(ClipMachineMemory, rd, rec, r, __PROC__);
}
