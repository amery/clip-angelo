int
clip_SQLFIELDNO(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   char *fieldname = _clip_parc(ClipMachineMemory, 2);

   int nfield = -1;

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }
   while ((++nfield < rowset->nfields) && (strncasecmp(rowset->fields[nfield].name, fieldname, MAXFIELDNAME)));
   if (nfield >= rowset->nfields)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOFIELD, er_nofield);
      return 1;
   }

   _clip_retni(ClipMachineMemory, nfield + 1);
   return 0;
}
