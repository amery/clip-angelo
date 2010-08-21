int
clip_SX_THERMOMETER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_WARM";

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retni(ClipMachineMemory, 0);
   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;

   if (wa->rd->orders[ord]->custom && !wa->rd->orders[ord]->canadd)
      _clip_retni(ClipMachineMemory, 4);
   else if (!wa->rd->orders[ord]->custom && !wa->rd->orders[ord]->canadd)
      _clip_retni(ClipMachineMemory, 3);
   else if (!wa->rd->orders[ord]->custom && wa->rd->orders[ord]->canadd)
      _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
