int
clip_DBWRITE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBWRITE";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar  *ap = _clip_par(ClipMachineMemory, 1);

   int       deleted = _clip_parl(ClipMachineMemory, 2);

   int       er;


   if (!wa)
      return rdd_err(ClipMachineMemory, EG_NOTABLE, 0, __FILE__, __LINE__, __PROC__, er_notable);
   CHECKARG1(1, MAP_type_of_ClipVarType);
   CHECKOPT1(2, LOGICAL_type_of_ClipVarType);

   WRITELOCK;
   if ((er = rdd_dbwrite(ClipMachineMemory, wa->rd, ap, __PROC__)))
      goto err_unlock;
   if (_clip_parinfo(ClipMachineMemory, 0) > 1)
    {
       if (deleted)
	{
	   if ((er = rdd_delete(ClipMachineMemory, wa->rd, __PROC__)))
	      goto err_unlock;
	}
       else
	{
	   if ((er = rdd_recall(ClipMachineMemory, wa->rd, __PROC__)))
	      goto err_unlock;
	}
    }
   UNLOCK;
   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
