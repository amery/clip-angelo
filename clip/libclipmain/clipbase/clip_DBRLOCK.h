int
clip_DBRLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBRLOCK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       rno = _clip_parni(ClipMachineMemory, 1);

   int       r, er;


   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      return clip_RLOCK(ClipMachineMemory);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_rlock(ClipMachineMemory, wa->rd, rno, &r, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, r);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
