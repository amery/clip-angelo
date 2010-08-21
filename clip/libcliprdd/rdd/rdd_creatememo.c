int
rdd_creatememo(ClipMachine * ClipMachineMemory, const char *driver, const char *name, const char *__PROC__)
{
   RDD_MEMO_VTBL *vtbl;

   char *path = NULL;

   int er = EG_UNSUPPORTED;

   if (!(vtbl = rdd_memodriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, vtbl->suff, &path, NULL, EG_CREATE, __PROC__)))
      goto err;

   if ((er = vtbl->create(ClipMachineMemory, path, __PROC__)))
      goto err;
   free(path);
   return 0;
 err:
   if (path)
      free(path);
   return er;
}
