int
clip_SX_INDEXNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_INDEXNAME";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   int ord, er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKOPT2(1, CHARACTER_type_of_ClipVarType, NUMERIC_type_of_ClipVarType);

   _clip_retc(ClipMachineMemory, "");
   ord = get_orderno(wa, order, NULL);
   if (ord == -1)
      ord = wa->rd->curord;
   if (ord == -1)
      return 0;
   _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->index->name);
   return 0;
 err:
   return er;
}
