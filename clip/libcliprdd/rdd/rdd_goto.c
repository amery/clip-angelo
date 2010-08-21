int
rdd_goto(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int rec, const char *__PROC__)
{
   int er;

   rd->pending_child_parent = NULL;
   if ((er = rdd_checkifnew(ClipMachineMemory, rd, __PROC__)))
      return er;

   if ((er = rd->vtbl->go(ClipMachineMemory, rd, rec, __PROC__)))
      return er;
/*	if(rd->curord != -1){
		if((er = rd->orders[rd->curord]->vtbl->buildtree(
			ClipMachineMemory,rd,rd->orders[rd->curord],__PROC__))) return er;
	}*/
   return 0;
}
