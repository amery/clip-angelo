int
clip_ISDBRLOCK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBRLOCK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       recno = _clip_parni(ClipMachineMemory, 1);

   int       r, er;


   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      recno = wa->rd->recno;

   if ((er = wa->rd->vtbl->rlocked(ClipMachineMemory, wa->rd, recno, &r, __PROC__)))
      goto err;
   _clip_retl(ClipMachineMemory, wa->rd->flocked || !wa->rd->shared || r);
   return 0;
 err:
   return er;
}
