int
clip_SQLTESTPARSER(ClipMachine * ClipMachineMemory)
{
   SQLCONN  *conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
						   _C_ITEM_TYPE_SQL);

   const char *sql = _clip_parc(ClipMachineMemory, 2);

   ClipVar  *ap = _clip_par(ClipMachineMemory, 3);

   char     *r = conn->vtbl->testparser(ClipMachineMemory, strdup(sql), ap);

   _clip_retcn_m(ClipMachineMemory, r, strlen(r));
   return 0;
}
