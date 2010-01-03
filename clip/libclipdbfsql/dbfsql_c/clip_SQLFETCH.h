int
clip_SQLFETCH(ClipMachine * ClipMachineMemory)
{
   int       rowset_item = _clip_parni(ClipMachineMemory, 1);

   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, rowset_item, _C_ITEM_TYPE_SQL);

   int       recs = _clip_parni(ClipMachineMemory, 2);

   ClipVar  *eval = _clip_vptr(_clip_par(ClipMachineMemory, 3));

   int       every = _clip_parni(ClipMachineMemory, 4);

   ClipVar  *ors = _clip_spar(ClipMachineMemory, 5);

   if (!rowset)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
       return 1;
    }
   if (!every)
      every = 1;
   if (rowset->conn->vtbl->fetch(ClipMachineMemory, rowset, recs, eval, every, ors))
      return 1;

   _clip_retni(ClipMachineMemory, rowset->loaded);
   return 0;
}
