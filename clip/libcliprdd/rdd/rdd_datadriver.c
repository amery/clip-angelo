RDD_DATA_VTBL *
rdd_datadriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__)
{
   int i;

   char err[256];

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_data_driver;
   for (i = 0; i < *ClipMachineMemory->ndata_drivers; i++)
   {
      if ((strlen(driver) == strlen((*ClipMachineMemory->data_drivers)[i]->id)) &&
	  (strncasecmp((*ClipMachineMemory->data_drivers)[i]->id, driver, sizeof((*ClipMachineMemory->data_drivers)[i]->id)) == 0))

	 return (*ClipMachineMemory->data_drivers)[i];
   }
   snprintf(err, sizeof(err), _clip_gettext("No data file driver linked: %s"), driver);
   rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, err);
   return NULL;
}
