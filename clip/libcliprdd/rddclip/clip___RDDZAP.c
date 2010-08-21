int
clip___RDDZAP(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "__RDDZAP";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_zap(ClipMachineMemory, rd, __PROC__)))
      goto err;
   return 0;
 err:
   return er;
}
