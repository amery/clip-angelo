int
clip_ORDLISTADD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDLISTADD";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   const char *bag = _clip_parc(ClipMachineMemory, 1);

   const char *ord = _clip_parc(ClipMachineMemory, 2);

   int       er;


   CHECKWA(wa);
   CHECKARG1(1, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   READLOCK;
   if ((er = rdd_setindex(ClipMachineMemory, wa->rd, NULL, wa->idx_driver, bag, ord, 0, __PROC__)))
      goto err_unlock;
   if ((er = rdd_gotop(ClipMachineMemory, wa->rd, __PROC__)))
      goto err_unlock;
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
