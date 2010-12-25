int
clip_BT_COUNT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_COUNT";

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
				  _C_ITEM_TYPE_BTREE);

   int er;

   CHECKBTREE(bt);
   _clip_retni(ClipMachineMemory, bt->in);
   return 0;
 err:
   return er;
}
