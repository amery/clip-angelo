int
clip_BT_DATA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_DATA";

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
				  _C_ITEM_TYPE_BTREE);

   int er;

   CHECKBTREE(bt);

   if (bt->type == 'I')
   {
      u4 *n = bt_key(bt);

      if (n)
      {
	 _clip_retni(ClipMachineMemory, *n);
      }
   }
   return 0;
 err:
   return er;
}
