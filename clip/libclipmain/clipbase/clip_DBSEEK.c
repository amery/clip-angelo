int
clip_DBSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSEEK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *v = _clip_par(ClipMachineMemory, 1);

   int soft = _clip_parl(ClipMachineMemory, 2);

   int last = _clip_parl(ClipMachineMemory, 3);

   int found, er;

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(3, LOGICAL_type_of_ClipVarType);

   if (!v)
      return 0;

   if (_clip_parinfo(ClipMachineMemory, 2) == UNDEF_type_of_ClipVarType)
      soft = (ClipMachineMemory->flags & SOFTSEEK_FLAG);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_seek(ClipMachineMemory, wa->rd, v, soft, last, &found, __PROC__)))
      goto err_unlock;
   UNLOCK;
   wa->found = (found ? 1 : 0);
   _clip_retl(ClipMachineMemory, found);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
