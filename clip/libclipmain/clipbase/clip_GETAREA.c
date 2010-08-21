int
clip_GETAREA(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "GETAREA";

   const char *fname = _clip_parc(ClipMachineMemory, 1);

   const char *driver = _clip_parc(ClipMachineMemory, 2);

   RDD_DATA_VTBL *vtbl;

   char *path = NULL;

   DBWorkArea *wa;

   int i, er = EG_UNSUPPORTED;

   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   if (!(vtbl = rdd_datadriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, fname, vtbl->suff, &path, NULL, EG_OPEN, __PROC__)))
      goto err;
   for (i = 0; i < ClipMachineMemory->areas->count_of_ClipVect; i++)
   {
      wa = ClipMachineMemory->areas->items_of_ClipVect[i];
      if (!wa || !wa->used)
	 continue;
      if (strcmp(wa->rd->path, path) == 0)
      {
	 _clip_retni(ClipMachineMemory, i + 1);
	 break;
      }
   }
   free(path);
   return 0;
 err:
   if (path)
      free(path);
   return er;
}
