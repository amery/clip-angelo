int
clip_SX_CHILL(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_CHILL";

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);
   ord = get_orderno(wa, order, index);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;

   wa->rd->orders[ord]->custom = 0;
   wa->rd->orders[ord]->canadd = 0;

   _clip_retl(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
