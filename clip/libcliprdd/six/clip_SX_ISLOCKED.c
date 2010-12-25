int
clip_SX_ISLOCKED(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_ISLOCKED";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int recno = _clip_parni(ClipMachineMemory, 1);

   int r = 0, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (!recno)
      recno = wa->rd->recno;

   if ((er = wa->rd->vtbl->rlocked(ClipMachineMemory, wa->rd, recno, &r, __PROC__)))
      goto err;
   if (!r)
   {
      if ((er = wa->rd->vtbl->forlock(ClipMachineMemory, wa->rd, recno, &r, __PROC__)))
	 goto err;
   }
   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}
