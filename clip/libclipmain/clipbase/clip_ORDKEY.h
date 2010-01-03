int
clip_ORDKEY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDKEY";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       ord = _clip_parni(ClipMachineMemory, 1);

   ClipVar  *order = _clip_par(ClipMachineMemory, 1);

   ClipVar  *index = _clip_par(ClipMachineMemory, 2);

   int       er;


   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType && !ord)
      ord = wa->rd->curord;
   else
      ord = get_orderno(wa, order, index);

   if (!(ord < 0 || ord >= wa->rd->ords_opened))
      _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->expr);
   return 0;
 err:
   return er;
}
