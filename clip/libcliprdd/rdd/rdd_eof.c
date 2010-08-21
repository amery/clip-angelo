int
rdd_eof(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *eof, const char *__PROC__)
{
   int er;

   if (rd->pending_child_parent)
      if ((er = rdd_child_duty(ClipMachineMemory, rd, __PROC__)))
	 return er;
   *eof = rd->eof;
   return 0;
}
