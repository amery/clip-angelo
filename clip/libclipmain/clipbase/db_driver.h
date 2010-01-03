int
db_driver(ClipMachine * ClipMachineMemory, const char *driver, char *data, char *idx, char *memo, const char *__PROC__)
{
   int       i;

   char      err[256];


   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_db_driver;
   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
    {
       if ((strlen(driver) == strlen((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver)) &&
	   (strncasecmp((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, driver,
			sizeof((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver)) == 0))
	{
	   if (data)
	      strcpy(data, (*ClipMachineMemory->dbdrivers)[i].data_of_DBDriver);
	   if (idx)
	      strcpy(idx, (*ClipMachineMemory->dbdrivers)[i].idx_of_DBDriver);
	   if (memo)
	      strcpy(memo, (*ClipMachineMemory->dbdrivers)[i].memo_of_DBDriver);
	   return 0;
	}
    }
   snprintf(err, sizeof(err), _clip_gettext("No driver linked: %s"), driver);
   return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, err);
}
