static int
dbf_rawgoto(ClipMachine * ClipMachineMemory, RDD_DATA * rd, unsigned int rec, char valid_stack, const char *__PROC__)
{
   int er;

   if (rd->valid && rd->changed)
      if ((er = dbf_setrecord(ClipMachineMemory, rd, __PROC__)))
	 return er;
   rd->valid = 0;
   rd->recno = rec;
   if (rd->curord >= 0)
      rd->orders[rd->curord]->valid_stack = valid_stack;
   return rdd_childs(ClipMachineMemory, rd, __PROC__);
}
