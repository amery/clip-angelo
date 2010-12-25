int
clip_SX_MEMOEXT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_MEMOEXT";

   const char *ext = _clip_parc(ClipMachineMemory, 1);

   int i, j, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT1(1, CHARACTER_type_of_ClipVarType);

   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
   {
      if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver, 6) == 0)
      {
	 for (j = 0; j < *ClipMachineMemory->nmemo_drivers; j++)
	 {
	    if (strncasecmp((*ClipMachineMemory->memo_drivers)[j]->id, (*ClipMachineMemory->dbdrivers)[i].memo_of_DBDriver, sizeof((*ClipMachineMemory->memo_drivers)[j]->id)) == 0)
	    {

	       _clip_retc(ClipMachineMemory, (*ClipMachineMemory->memo_drivers)[j]->suff);
	       if (ext && ext[0])
	       {
		  memset((*ClipMachineMemory->memo_drivers)[j]->suff, 0, 5);
		  strncpy((*ClipMachineMemory->memo_drivers)[j]->suff, ext, min(4, strlen(ext)));
		  for (i = 0; i < 4; i++)
		     (*ClipMachineMemory->memo_drivers)[j]->suff[i] = tolower((*ClipMachineMemory->memo_drivers)[j]->suff[i]);
	       }
	       break;
	    }
	 }
	 break;
      }
   }
   return 0;
 err:
   return er;
}
