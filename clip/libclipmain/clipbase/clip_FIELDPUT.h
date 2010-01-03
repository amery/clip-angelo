int
clip_FIELDPUT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDPUT";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       fno = _clip_parni(ClipMachineMemory, 1);

   ClipVar  *vp = _clip_par(ClipMachineMemory, 2);

   int       er;


   if (!wa)
      return 0;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (fno < 1 || fno > wa->rd->nfields)
      return 0;

   WRITELOCK;
   if ((er = rdd_setvalue(ClipMachineMemory, wa->rd, fno - 1, vp, __PROC__)))
      goto err_unlock;
   UNLOCK;
   _clip_clone(ClipMachineMemory, RETPTR(ClipMachineMemory), vp);
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
