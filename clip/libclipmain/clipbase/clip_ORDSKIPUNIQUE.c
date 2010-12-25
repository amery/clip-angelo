int
clip_ORDSKIPUNIQUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDSKIPUNIQUE";

   int recs = _clip_parni(ClipMachineMemory, 1);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar prev;

   ClipVar curr;

   int c, er;

   _clip_retl(ClipMachineMemory, 0);
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (recs >= 0)
      recs = 1;
   else
      recs = -1;

   _clip_retl(ClipMachineMemory, 1);

   READLOCK;
   if ((er = rdd_keyvalue(ClipMachineMemory, wa->rd, &prev, __PROC__)))
      goto err_unlock;
   while (1)
   {
      if (wa->rd->bof || wa->rd->eof)
      {
	 UNLOCK;
	 return 0;
      }
      if ((er = rdd_skip(ClipMachineMemory, wa->rd, recs, __PROC__)))
	 goto err_unlock;
      if ((er = rdd_keyvalue(ClipMachineMemory, wa->rd, &curr, __PROC__)))
	 goto err_unlock;
      _clip_cmp(ClipMachineMemory, &prev, &curr, &c, 1);
      _clip_destroy(ClipMachineMemory, &prev);
      _clip_clone(ClipMachineMemory, &prev, &curr);
      if (!c)
	 break;
   }
   UNLOCK;
   _clip_destroy(ClipMachineMemory, &prev);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
