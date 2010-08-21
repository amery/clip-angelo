int
rdd_ii_close(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   RDD_INDEX *ri = (RDD_INDEX *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_INDEX);

   int i;

   int er;

   CHECKII(ri);
   for (i = 0; i < ri->norders; i++)
   {
      if ((er = rdd_ii_closetag(ClipMachineMemory, ri->orders[i]->c_item, __PROC__)))
	 return er;
   }
   _clip_destroy_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_INDEX);
   return 0;
}
