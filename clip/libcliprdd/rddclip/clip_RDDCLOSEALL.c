int
clip_RDDCLOSEALL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCLOSEALL";

   RDD_DATA *rd;

   int key, i, er;

   for (i = 0; i < ClipMachineMemory->container->len_of_Container; i++)
   {
      if (ClipMachineMemory->container->ContainerItem_items_of_Container[i].type_of_ContainerItem == _C_ITEM_TYPE_RDD)
      {
	 rd = (RDD_DATA *) ClipMachineMemory->container->ContainerItem_items_of_Container[i].item_of_ContainerItem;
	 if (rd->ClipMachineMemory == ClipMachineMemory && rd->area == -1)
	 {
	    key = ClipMachineMemory->container->ContainerItem_items_of_Container[i].key_of_ContainerItem;
	    if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
	       goto err;
	    if (rd->readonly)
	       rd->vtbl->_rlock(ClipMachineMemory, rd, __PROC__);
	    else
	       rd->vtbl->_wlock(ClipMachineMemory, rd, __PROC__);
	    if ((er = rdd_closearea(ClipMachineMemory, rd, __PROC__)))
	       goto err;
	    _clip_destroy_c_item(ClipMachineMemory, key, _C_ITEM_TYPE_RDD);
	    rd = (RDD_DATA *) _clip_fetch_c_item_type(ClipMachineMemory, _C_ITEM_TYPE_RDD, &key);
	 }
      }
   }
   return 0;
 err:
   return er;
}
