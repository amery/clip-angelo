int
clip_ISDBRLOCKFOREIGN(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ISDBRLOCKFOREIGN";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       recno = _clip_parni(ClipMachineMemory, 1);

   int       er, r;


   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      recno = wa->rd->recno;

   if ((er = wa->rd->vtbl->forlock(ClipMachineMemory, wa->rd, recno, &r, __PROC__)))
      goto err;

   _clip_retl(ClipMachineMemory, r);
   return 0;
 err:
   return er;
}
