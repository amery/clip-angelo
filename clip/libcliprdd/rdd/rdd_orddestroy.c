int
rdd_orddestroy(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   int i, er;

   if ((er = ro->vtbl->destroy(ClipMachineMemory, rd, ro, __PROC__)))
      return er;

   for (i = 0; i < rd->ords_opened; i++)
   {
      if (rd->orders[i] == ro)
      {
	 for (i++; i < rd->ords_opened; i++)
	 {
	    rd->orders[i - 1] = rd->orders[i];
	    rd->orders[i - 1]->orderno = i - 1;
	 }
	 rd->ords_opened--;
	 rd->orders = (RDD_ORDER **) realloc(rd->orders, rd->ords_opened * sizeof(RDD_ORDER *));
	 break;
      }
   }

   for (i = 0; i < ro->index->norders; i++)
   {
      if (ro->index->orders[i] == ro)
      {
	 for (i++; i < ro->index->norders; i++)
	 {
	    ro->index->orders[i - 1] = ro->index->orders[i];
	 }
	 ro->index->norders--;
	 ro->index->orders = (RDD_ORDER **) realloc(ro->index->orders, ro->index->norders * sizeof(RDD_ORDER *));
	 break;
      }
   }

   destroy_rdd_order(ClipMachineMemory, ro);
   rd->curord = -1;
   return 0;
}
