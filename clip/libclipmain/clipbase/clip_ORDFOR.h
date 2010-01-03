int
clip_ORDFOR(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDFOR";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar  *order = _clip_par(ClipMachineMemory, 1);

   ClipVar  *index = _clip_par(ClipMachineMemory, 2);

   int       ord, er;


   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;

   _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->cforexpr);
   return 0;
 err:
   return er;
}
