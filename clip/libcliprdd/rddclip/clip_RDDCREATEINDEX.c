int
clip_RDDCREATEINDEX(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDDCREATEINDEX";

   RDD_DATA *rd = _fetch_rdd(ClipMachineMemory, __PROC__);

   const char *driver = _clip_parc(ClipMachineMemory, 2);

   const char *name = _clip_parc(ClipMachineMemory, 3);

   const char *tag = _clip_parc(ClipMachineMemory, 4);

   const char *expr = _clip_parc(ClipMachineMemory, 5);

   int unique = _clip_parl(ClipMachineMemory, 6);

   int lIgnoreCase = _clip_parl(ClipMachineMemory, 7);

   const char *cfor = _clip_parc(ClipMachineMemory, 8);

   int er;

   if (!rd)
      return EG_NOTABLE;
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, CHARACTER_type_of_ClipVarType);
   CHECKARG1(5, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(6, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(7, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(8, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 6) == UNDEF_type_of_ClipVarType)
      unique = ClipMachineMemory->flags & UNIQUE_FLAG;

   rd->os.lIgnoreCase = lIgnoreCase;

   if (rd->os.cForCondition)
   {
      free(rd->os.cForCondition);
      rd->os.cForCondition = NULL;
   }
   if (cfor && cfor[0])
   {
      int r = _clip_parni(ClipMachineMemory, 1);

      char forexpr[1024];

      rd->os.cForCondition = malloc(strlen(cfor) + 1);
      strcpy(rd->os.cForCondition, cfor);

      memset(forexpr, 0, sizeof(forexpr));

      rdd_expandmacro(rd->area, r, cfor, forexpr);

      if ((er = _clip_eval_macro(ClipMachineMemory, forexpr, strlen(forexpr), &rd->os.bForCondition)))
	 goto err;
   }

   if ((er = rdd_flushbuffer(ClipMachineMemory, rd, __PROC__)))
      goto err;
   if (rd->readonly)
   {
      READLOCK;
   }
   else
   {
      WRITELOCK;
   }
   if ((er = rdd_createindex(ClipMachineMemory, rd, driver, name, tag, expr, NULL, unique, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   rd->vtbl->_ulock(ClipMachineMemory, rd, __PROC__);
 err:
   return er;
}
