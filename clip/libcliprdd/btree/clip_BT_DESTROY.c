int
clip_BT_DESTROY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_DESTROY";

   int h = _clip_parni(ClipMachineMemory, 1);

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_BTREE);

   int er;

   CHECKBTREE(bt);

   _clip_destroy_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_BTREE);
   return 0;
 err:
   return er;
}
