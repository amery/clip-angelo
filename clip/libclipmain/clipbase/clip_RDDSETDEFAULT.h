int
clip_RDDSETDEFAULT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDSETDEFAULT";

   const char *driver = _clip_parc(ClipMachineMemory, 1);

   int       er;


   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);

   _clip_retc(ClipMachineMemory, ClipMachineMemory->def_db_driver);

   if (driver && driver[0])
    {
       int       i;

       for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
	{
	   if (strncasecmp(driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver,
			   sizeof((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver)) == 0)
	    {
	       strncpy(ClipMachineMemory->def_db_driver, driver, sizeof((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver));
	       ClipMachineMemory->def_db_driver[8] = 0;
	       strcpy(ClipMachineMemory->def_data_driver, (*ClipMachineMemory->dbdrivers)[i].data_of_DBDriver);
	       strcpy(ClipMachineMemory->def_idx_driver, (*ClipMachineMemory->dbdrivers)[i].idx_of_DBDriver);
	       strcpy(ClipMachineMemory->def_memo_driver, (*ClipMachineMemory->dbdrivers)[i].memo_of_DBDriver);
	       break;
	    }
	}
    }
   return 0;
 err:
   return er;
}
