int
rdd_ii_id(ClipMachine * ClipMachineMemory, int h, const char *__PROC__)
{
   ClipVar *id = RETPTR(ClipMachineMemory);

   RDD_ORDER *ro = (RDD_ORDER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_I_ORDER);

   CHECKIO(ro);
   return ro->vtbl->ii_id(ClipMachineMemory, ro, id, __PROC__);
}
