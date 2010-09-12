int
clip_ORDKEYGOTO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEYGOTO";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   unsigned int nkey = _clip_parni(ClipMachineMemory, 1);

   int ok, er;

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (wa->rd->curord == -1)
      return 0;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if ((er = rdd_gotokey(ClipMachineMemory, wa->rd, wa->rd->orders[wa->rd->curord], nkey, &ok, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_retl(ClipMachineMemory, ok);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
