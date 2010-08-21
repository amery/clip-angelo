int
clip_BT_CREATE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "BT_CREATE";

   u4 count = _clip_parni(ClipMachineMemory, 1);

   int itemsize = _clip_parni(ClipMachineMemory, 2);

   const char *type = _clip_parc(ClipMachineMemory, 3);

   BTREE *bt;

   int (*compare) (void *op, void *key1, void *key2, int *uniqfound) = NULL;

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, NUMERIC_type_of_ClipVarType);
   CHECKOPT3(3, CHARACTER_type_of_ClipVarType, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      itemsize = 4;
   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      type = "I";

   if (toupper(type[0]) == 'I' && itemsize == 4)
   {
      compare = _comp_uint;
   }

   if (!compare)
   {
      er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_unknowndata);
      goto err;
   }
   bt = bt_create(1, count, itemsize, compare);
   bt->type = toupper(type[0]);
   _clip_retni(ClipMachineMemory, _clip_store_c_item(ClipMachineMemory, bt, _C_ITEM_TYPE_BTREE, destroy_btree));
   return 0;
 err:
   return er;
}
