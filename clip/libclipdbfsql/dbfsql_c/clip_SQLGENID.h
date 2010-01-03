int
clip_SQLGENID(ClipMachine * ClipMachineMemory)
{
   int       rowset_item = _clip_parni(ClipMachineMemory, 1);

   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }

   if (rowset->conn->vtbl->genid)
      return rowset->conn->vtbl->genid(ClipMachineMemory, rowset);
   return 0;
}
