int
clip_SX_WILDSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_WILDSEEK";

   const char *pattern = _clip_parc(ClipMachineMemory, 1);

   int cont = _clip_parl(ClipMachineMemory, 2);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int found, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   if (_clip_parinfo(ClipMachineMemory, 1) != CHARACTER_type_of_ClipVarType)
   {
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);

   if ((er = rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__)))
      return er;
   READLOCK;
   if (rdd_wildseek(ClipMachineMemory, wa->rd, pattern, 0, cont, &found, __PROC__))
      goto err_unlock;
   UNLOCK;

   wa->found = found;
   _clip_retl(ClipMachineMemory, found);
   return 0;

 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
