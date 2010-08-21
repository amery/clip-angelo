int
rdd_keyno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *keyno, const char *__PROC__)
{
   int er;

   if ((rd->ords_opened < 1) || (rd->curord == -1))
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);

   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;
   if ((er = rd->orders[rd->curord]->vtbl->keyno(ClipMachineMemory, rd, rd->orders[rd->curord], keyno, __PROC__)))
      return er;
   return 0;
}
