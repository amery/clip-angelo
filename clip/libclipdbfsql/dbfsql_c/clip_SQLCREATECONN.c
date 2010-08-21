int
clip_SQLCREATECONN(ClipMachine * ClipMachineMemory)
{
   char *id = _clip_parc(ClipMachineMemory, 1);

   int conn_item = -1;

   SQLCONN *conn;

   const char *sqlcs = _clip_parc(ClipMachineMemory, 9);

   int i;

   for (i = 0; i < (*ClipMachineMemory->nsqldrivers); i++)
   {
      if (strcasecmp(id, (*ClipMachineMemory->sqldrivers)[i].id_of_SQLDriver) == 0)
      {
	 conn_item = (*(*ClipMachineMemory->sqldrivers)[i].connect) (ClipMachineMemory);
	 if (conn_item == -1)
	 {
	    return 1;
	 }
	 break;
      }
   }
   if (i == *ClipMachineMemory->nsqldrivers)
   {
      char err[256];

      snprintf(err, sizeof(err), ";Unknown RDBMS (bad identifier '%s' or library not linked);", id);
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOLIB, err);
      return 1;
   }

   conn = (SQLCONN *) _clip_fetch_c_item(ClipMachineMemory, conn_item, _C_ITEM_TYPE_SQL);
   conn->loc = SQL_get_locale(ClipMachineMemory, sqlcs);

   _clip_retni(ClipMachineMemory, conn_item);
   return 0;
}
