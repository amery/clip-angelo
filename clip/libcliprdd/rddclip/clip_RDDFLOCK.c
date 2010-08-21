int
clip_RDDFLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDFLOCK";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_flock(ClipMachineMemory, rd, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
