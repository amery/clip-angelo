int
clip_DBAPPEND(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBAPPEND";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       rellocks = _clip_parl(ClipMachineMemory, 1);

   ClipVar  *ap = _clip_par(ClipMachineMemory, 2);

   int      *neterr = &ClipMachineMemory->neterr;

   int       er;


   CHECKWA(wa);
   CHECKOPT1(1, LOGICAL_type_of_ClipVarType);
   CHECKOPT1(2, MAP_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      rellocks = 1;

   if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
      goto err;
   WRITELOCK;
   if (rellocks)
    {
       if ((er = rdd_ulock(ClipMachineMemory, wa->rd, 0, 0, __PROC__)))
	  goto err_unlock;
    }
   if ((er = rdd_append(ClipMachineMemory, wa->rd, neterr, __PROC__)))
      goto err_unlock;
   if (!(*neterr))
    {
       if (ap && ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	  if ((er = rdd_dbwrite(ClipMachineMemory, wa->rd, ap, __PROC__)))
	     goto err_unlock;
    }
   UNLOCK;
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
