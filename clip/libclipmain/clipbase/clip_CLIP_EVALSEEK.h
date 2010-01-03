int
clip_CLIP_EVALSEEK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "CLIP_EVALSEEK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar  *block = _clip_spar(ClipMachineMemory, 1);

   int       found, er;


   CHECKWA(wa);
   CHECKARG2(1, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   READLOCK;
   if (rdd_seekeval(ClipMachineMemory, wa->rd, block, &found, __PROC__))
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
