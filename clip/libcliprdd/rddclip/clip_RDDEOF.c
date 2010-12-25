int
clip_RDDEOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDEOF";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int eof, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_eof(ClipMachineMemory, rd, &eof, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, eof);
   return 0;
 err:
   return er;
}
