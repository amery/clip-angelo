int
clip_SX_TAGUNIQUE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_TAGUNIQUE";

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);
   CHECKOPT2(2, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retl(ClipMachineMemory, 0);
   if (!wa)
      return 0;

   ord = get_orderno(wa, order, index);
   if (ord == -1)
      return 0;

   _clip_retl(ClipMachineMemory, wa->rd->orders[ord]->unique);
   return 0;
 err:
   return er;
}
