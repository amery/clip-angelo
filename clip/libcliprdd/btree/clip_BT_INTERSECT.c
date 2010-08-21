int
clip_BT_INTERSECT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_INTERSECT";

   BTREE *bt = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
				  _C_ITEM_TYPE_BTREE);

   BTREE *b1 = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 2),
				  _C_ITEM_TYPE_BTREE);

   BTREE *b2 = _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 3),
				  _C_ITEM_TYPE_BTREE);

   int e1 = 0, e2 = 0, er;

   u4 d1, d2;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG1(2, NUMERIC_type_of_ClipVarType);
   CHECKARG1(3, NUMERIC_type_of_ClipVarType);
   CHECKBTREE(bt);
   CHECKBTREE(b1);
   CHECKBTREE(b2);

   e1 = bt_first(b1);
   e2 = bt_first(b2);
   while (!e1 && !e2)
   {
      d1 = *(u4 *) bt_key(b1);
      d2 = *(u4 *) bt_key(b2);
      if (d1 == d2)
      {
	 if (bt_add(bt, NULL, &d1))
	 {
	    er = rdd_err(ClipMachineMemory, -1, 0, __FILE__, __LINE__, __PROC__, er_internal);
	    goto err;
	 }
	 e1 = bt_next(b1);
	 e2 = bt_next(b2);
      }
      else if (d1 < d2)
      {
	 e1 = bt_next(b1);
      }
      else
      {
	 e2 = bt_next(b2);
      }
   }
   return 0;
 err:
   return er;
}
