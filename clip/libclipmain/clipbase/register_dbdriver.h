void
register_dbdriver(ClipMachine * ClipMachineMemory, char *id, char *data, char *idx, char *memo)
{
   if (!*ClipMachineMemory->dbdrivers)
      *ClipMachineMemory->ndbdrivers = 0;
   if (!*ClipMachineMemory->ndbdrivers)
      *ClipMachineMemory->dbdrivers = (DBDriver *) malloc(sizeof(DBDriver));
   else
      *ClipMachineMemory->dbdrivers = (DBDriver *) realloc(*ClipMachineMemory->dbdrivers,
							   sizeof(DBDriver) * (*ClipMachineMemory->ndbdrivers + 1));
   (*ClipMachineMemory->ndbdrivers)++;
   strcpy((*ClipMachineMemory->dbdrivers)[*ClipMachineMemory->ndbdrivers - 1].id_of_DBDriver, id);
   strcpy((*ClipMachineMemory->dbdrivers)[*ClipMachineMemory->ndbdrivers - 1].data_of_DBDriver, data);
   strcpy((*ClipMachineMemory->dbdrivers)[*ClipMachineMemory->ndbdrivers - 1].idx_of_DBDriver, idx);
   strcpy((*ClipMachineMemory->dbdrivers)[*ClipMachineMemory->ndbdrivers - 1].memo_of_DBDriver, memo);
}
