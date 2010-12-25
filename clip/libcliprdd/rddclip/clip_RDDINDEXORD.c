int
clip_RDDINDEXORD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDINDEXORD";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   _clip_retni(ClipMachineMemory, rd->curord + 1);
   return 0;
}
