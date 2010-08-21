int
rdd_ii_closetag(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   int i;

   CHECKIO(ro);
   for (i = 0; i < ro->index->norders; i++)
   {
      if (ro->index->orders[i] == ro)
      {
	 for (i++; i < ro->index->norders; i++)
	 {
	    ro->index->orders[i - 1] = ro->index->orders[i];
	 }
      }
   }
   ro->index->norders--;
   ro->index->orders = (RDD_ORDER **) realloc(ro->index->orders, ro->index->norders * sizeof(RDD_ORDER *));
   destroy_rdd_order(ClipMachineMemory, ro);
   _clip_destroy_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);
   return 0;
}
