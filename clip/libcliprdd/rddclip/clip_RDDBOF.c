int
clip_RDDBOF(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDBOF";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int bof, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_bof(ClipMachineMemory, rd, &bof, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, bof);
   return 0;
 err:
   return er;
}
