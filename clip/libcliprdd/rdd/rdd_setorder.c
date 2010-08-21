int
rdd_setorder(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int order, const char *__PROC__)
{
   int er;

   if (order < 0 || order > rd->ords_opened || order - 1 == rd->curord)
      return 0;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   rd->curord = order - 1;
   if (rd->curord != -1)
      rd->orders[rd->curord]->valid_stack = 0;
   if ((er = _rdd_calcfiltlist(ClipMachineMemory, rd, rd->filter, __PROC__)))
      return er;
   return 0;
}
