int
clip_TABLEEXT(ClipMachine * ClipMachineMemory)
{
   int i, j;

   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
   {
      if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, 6) == 0)
      {
	 for (j = 0; j < *ClipMachineMemory->ndata_drivers; j++)
	 {
	    if (strncasecmp((*ClipMachineMemory->data_drivers)[j]->id, (*ClipMachineMemory->dbdrivers)[i].data_of_DBDriver, sizeof((*ClipMachineMemory->data_drivers)[j]->id)) == 0)
	    {

	       _clip_retc(ClipMachineMemory, (*ClipMachineMemory->data_drivers)[j]->suff);
	       break;
	    }
	 }
	 break;
      }
   }
   return 0;
}
