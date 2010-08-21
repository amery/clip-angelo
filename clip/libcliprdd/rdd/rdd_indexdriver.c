RDD_INDEX_VTBL *
rdd_indexdriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__)
{
   int i;

   char err[256];

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_idx_driver;
   for (i = 0; i < *ClipMachineMemory->nidx_drivers; i++)
   {
      if ((strlen(driver) == strlen((*ClipMachineMemory->idx_drivers)[i]->id)) &&
	  (strncasecmp((*ClipMachineMemory->idx_drivers)[i]->id, driver, sizeof((*ClipMachineMemory->idx_drivers)[i]->id)) == 0))

	 return (*ClipMachineMemory->idx_drivers)[i];
   }
   snprintf(err, sizeof(err), _clip_gettext("No index file driver linked: %s"), driver);
   rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, err);
   return NULL;
}
