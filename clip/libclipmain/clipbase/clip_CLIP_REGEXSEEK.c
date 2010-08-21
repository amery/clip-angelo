int
clip_CLIP_REGEXSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_REGEXSEEK";

   const char *pattern = _clip_parc(ClipMachineMemory, 1);

   int cont = _clip_parl(ClipMachineMemory, 2);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int found, er;

   CHECKWA(wa);
   if (_clip_parinfo(ClipMachineMemory, 1) != CHARACTER_type_of_ClipVarType)
   {
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (rdd_wildseek(ClipMachineMemory, wa->rd, pattern, 1, cont, &found, __PROC__))
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
