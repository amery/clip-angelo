int
rdd_flock(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int r, er;

   if ((er = rd->vtbl->flock(ClipMachineMemory, rd, &r, __PROC__)))
      return er;
   _clip_retl(ClipMachineMemory, r);
   return 0;
}
