int
clip_RDDUSEAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDUSEAREA";

   const char *driver = _clip_parc(ClipMachineMemory, 1);

   const char *name = _clip_parc(ClipMachineMemory, 2);

   int shared = _clip_parl(ClipMachineMemory, 3);

   int readonly = _clip_parl(ClipMachineMemory, 4);

   int tempo = _clip_parl(ClipMachineMemory, 5);

   RDD_DATA *data;

   int er;

   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(5, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 3) == UNDEF_type_of_ClipVarType)
      shared = !(ClipMachineMemory->flags & EXCLUSIVE_FLAG);

   if ((er = rdd_usearea(ClipMachineMemory, driver, name, shared, readonly, tempo, &data, __PROC__)))
      goto err;
   data->area = -1;
   data->rdhandle = _clip_store_c_item(ClipMachineMemory, (void *) data, _C_ITEM_TYPE_RDD, destroy_rdd_data);
   _clip_retni(ClipMachineMemory, data->rdhandle);
   return 0;
 err:
   return er;
}
