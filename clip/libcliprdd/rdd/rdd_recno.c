int
rdd_recno(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *recno, const char *__PROC__)
{
   int er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;
   *recno = rd->recno;
   return 0;
}
