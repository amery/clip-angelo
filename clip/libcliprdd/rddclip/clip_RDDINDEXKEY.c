int
clip_RDDINDEXKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDINDEXKEY";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   if (!rd)
      return EG_NOTABLE;

   if (rd->curord != -1)
      _clip_retc(ClipMachineMemory, rd->orders[rd->curord]->expr);
   return 0;
}
