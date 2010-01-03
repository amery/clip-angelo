int
clip_INDEXEXT(ClipMachine * ClipMachineMemory)
{
   int       i, j;


   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
    {
       if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, 6) == 0)
	{
	   for (j = 0; j < *ClipMachineMemory->nidx_drivers; j++)
	    {
	       if (strncasecmp
		   ((*ClipMachineMemory->idx_drivers)[j]->id,
		    (*ClipMachineMemory->dbdrivers)[i].idx_of_DBDriver, sizeof((*ClipMachineMemory->idx_drivers)[j]->id)) == 0)
		{

		   _clip_retc(ClipMachineMemory, (*ClipMachineMemory->idx_drivers)[j]->suff);
		   break;
		}
	    }
	   break;
	}
    }
   return 0;
}
