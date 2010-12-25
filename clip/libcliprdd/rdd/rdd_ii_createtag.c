int
rdd_ii_createtag(ClipMachine * ClipMachineMemory, int h, const char *tag, const char *expr, const char *__PROC__)
{
   RDD_INDEX *ri = (RDD_INDEX *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_INDEX);

   RDD_ORDER *ro;

   int er;

   CHECKII(ri);

   if ((er = ri->vtbl->ii_createtag(ClipMachineMemory, ri, tag, expr, &ro, __PROC__)))
      return er;

   ro->c_item = _clip_store_c_item(ClipMachineMemory, ro, _C_ITEM_TYPE_I_ORDER, destroy_rdd_i_order);
   _clip_retni(ClipMachineMemory, ro->c_item);
   return 0;
}
