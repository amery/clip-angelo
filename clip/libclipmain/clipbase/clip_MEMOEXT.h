int
clip_MEMOEXT(ClipMachine * ClipMachineMemory)
{
   int       i, j;

   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
    {
       if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, 6) == 0)
	{
	   for (j = 0; j < *ClipMachineMemory->nmemo_drivers; j++)
	    {
	       if (strncasecmp
		   ((*ClipMachineMemory->memo_drivers)[j]->id,
		    (*ClipMachineMemory->dbdrivers)[i].memo_of_DBDriver,
		    sizeof((*ClipMachineMemory->memo_drivers)[j]->id)) == 0)
		{

		   _clip_retc(ClipMachineMemory, (*ClipMachineMemory->memo_drivers)[j]->suff);
		   break;
		}
	    }
	   break;
	}
    }
   return 0;
}
