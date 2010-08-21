int
clip_BT_ADD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_ADD";

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
				  _C_ITEM_TYPE_BTREE);

   int er;

   CHECKBTREE(bt);

   if (bt->type == 'I')
   {
      u4 n = _clip_parni(ClipMachineMemory, 2);

      CHECKARG1(2, NUMERIC_type_of_ClipVarType);

      if (bt_add(bt, NULL, &n))
      {
	 er = rdd_err(ClipMachineMemory, -1, 0, __FILE__, __LINE__, __PROC__, er_internal);
	 goto err;
      }
   }
   return 0;
 err:
   return er;
}
