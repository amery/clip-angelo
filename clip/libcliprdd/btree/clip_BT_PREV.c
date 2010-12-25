int
clip_BT_PREV(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_PREV";

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
				  _C_ITEM_TYPE_BTREE);

   int er;

   CHECKBTREE(bt);
   _clip_retl(ClipMachineMemory, !bt_prev(bt));
   return 0;
 err:
   return er;
}
