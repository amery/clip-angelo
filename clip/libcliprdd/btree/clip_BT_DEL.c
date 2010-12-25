int
clip_BT_DEL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_DEL";

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
				  _C_ITEM_TYPE_BTREE);

   int er;

   CHECKBTREE(bt);

   if (bt_del(bt))
   {
      er = rdd_err(ClipMachineMemory, -1, 0, __FILE__, __LINE__, __PROC__, er_internal);
      goto err;
   }
   return 0;
 err:
   return er;
}
