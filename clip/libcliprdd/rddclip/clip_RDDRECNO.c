int
clip_RDDRECNO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDRECNO";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   int recno, er;

   if (!rd)
      return EG_NOTABLE;

   if ((er = rdd_recno(ClipMachineMemory, rd, &recno, __PROC__)))
      goto err;
   _clip_retni(ClipMachineMemory, recno);
   return 0;
 err:
   return er;
}
