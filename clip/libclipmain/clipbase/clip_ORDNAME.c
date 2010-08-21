int
clip_ORDNAME(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "ORDNAME";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int ord = _clip_parni(ClipMachineMemory, 1);

   ClipVar *order = _clip_par(ClipMachineMemory, 1);

   ClipVar *index = _clip_par(ClipMachineMemory, 2);

   int er;

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   if (!ord)
      ord = wa->rd->curord;
   else
      ord = get_orderno(wa, order, index);
   if (ord == -1)
      return 0;

   _clip_retc(ClipMachineMemory, wa->rd->orders[ord]->name);
   return 0;
 err:
   return er;
}
