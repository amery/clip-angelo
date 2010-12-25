RDD_MEMO_VTBL *
rdd_memodriver(ClipMachine * ClipMachineMemory, const char *driver, const char *__PROC__)
{
   int i;

   char err[256];

   if (!driver || !driver[0])
      driver = ClipMachineMemory->def_memo_driver;
   for (i = 0; i < *ClipMachineMemory->nmemo_drivers; i++)
   {
      if ((strlen(driver) == strlen((*ClipMachineMemory->memo_drivers)[i]->id)) &&
	  (strncasecmp((*ClipMachineMemory->memo_drivers)[i]->id, driver, sizeof((*ClipMachineMemory->memo_drivers)[i]->id)) == 0))

	 return (*ClipMachineMemory->memo_drivers)[i];
   }
   snprintf(err, sizeof(err), _clip_gettext("No memo file driver linked: %s"), driver);
   rdd_err(ClipMachineMemory, EG_UNSUPPORTED, 0, __FILE__, __LINE__, __PROC__, err);
   return NULL;
}
