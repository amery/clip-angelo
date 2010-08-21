int
rdd_keyvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, ClipVar * v, const char *__PROC__)
{
   if (rd->curord == -1)
      return rdd_err(ClipMachineMemory, EG_NOORDER, 0, __FILE__, __LINE__, __PROC__, er_noorder);
   return rd->orders[rd->curord]->vtbl->keyvalue(ClipMachineMemory, rd, rd->orders[rd->curord], v, __PROC__);
}
