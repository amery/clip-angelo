int
clip_BT_FIRST(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_FIRST";

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
				  _C_ITEM_TYPE_BTREE);

   int er;

   CHECKBTREE(bt);
   _clip_retl(ClipMachineMemory, !bt_first(bt));
   return 0;
 err:
   return er;
}
